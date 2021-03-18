const http = require("http");
const os = require("os");
const fs = require("fs");
const iolib = require("socket.io");
const osc = require("node-osc");
const yargs = require("yargs");

const argv = yargs
  .command(
    "webmixer",
    "Webmixer transfers local OSC messages and provide a websocket bound frontend"
  )
  .option("records", {
    alias: "r",
    description: "Specifies the path to the records",
    type: "string",
  })
  .option("ip", {
    alias: "i",
    description: "Specifies the ip address",
    type: "string",
  })
  .option("name", {
    alias: "n",
    description: "Specifies the device name",
    type: "string",
  })
  .option("port", {
    alias: "p",
    description: "Specifies the listen port",
    type: "number",
  })
  .help()
  .alias("help", "h").argv;

const homedir = argv.records || require("os").homedir();

const port = argv.port || 8000;

//oscClient2 = new osc.Client( 'localhost', 9872 );

const httpserver = http.createServer(function (req, res) {
  // check if file is in local directory:
  if (req.url.startsWith("/rec") && req.url.endsWith(".wav")) {
    // download from local directory:
    if (fs.existsSync("." + req.url)) {
      const data = fs.readFileSync("." + req.url);
      res.writeHead(200);
      res.end(data);
      return;
    }
    // check in home directory:
    if (fs.existsSync(homedir + req.url)) {
      const data = fs.readFileSync(homedir + req.url);
      res.writeHead(200);
      res.end(data);
      return;
    }
  }
  const hosjs = fs.readFileSync("assets/ovclient.js");
  const hoscss = fs.readFileSync("assets/ovclient.css");
  const jackrec = fs.readFileSync("assets/jackrec.html");
  const ipaddr = argv.ip || os.hostname();
  const devname = argv.name || os.hostname();
  try {
    devname = fs.readFileSync("devicename");
  } catch (ee) {}
  res.writeHead(200, { "Content-Type": "text/html" });
  res.write("<!DOCTYPE HTML>\n");
  res.write('<html lang="en"><head><title>Mixer</title><style>');
  res.write(hoscss);
  res.write("</style><title>ov-client web mixer</title>\n</head><body>\n");
  res.write("<h1>" + devname + '</h1>\n<div id="mixer">mixer</div>\n');
  res.write(
    '<script src="http://' +
      ipaddr +
      ':8080/socket.io/socket.io.js"></script>\n'
  );
  res.write("<script>\n");
  res.write('var socket = io("http://' + ipaddr + ':8080");\n');
  res.write(hosjs);
  res.write("</script>\n");
  res.write(jackrec);
  res.end("</body></html>");
});
console.log("Serving records at " + homedir);

httpserver.listen(port);
const io = iolib(httpserver);

const oscServer = new osc.Server(9000, "0.0.0.0");
const oscClient = new osc.Client("localhost", 9871);

io.on("connection", function (socket) {
  socket.on("config", function () {
    oscClient.send("/status", socket.id + " connected");
    oscServer.on("message", async function (msg) {
      if (msg[0] === "/touchosc/scene") {
        socket.emit("scene", "scene");
      }
      if (
        msg[0].startsWith("/touchosc/label") &&
        !msg[0].endsWith("/color") &&
        msg[1].length > 1
      ) {
        socket.emit("newfader", msg[0].substr(15), msg[1]);
      }
      if (msg[0].startsWith("/touchosc/fader") && !msg[0].endsWith("/color")) {
        socket.emit("updatefader", msg[0], msg[1]);
      }
      if (msg[0].startsWith("/touchosc/level")) {
        socket.emit("updatefader", msg[0], msg[1]);
      }
      if (msg[0] === "/jackrec/start") socket.emit("jackrecstart", "");
      if (msg[0] === "/jackrec/stop") socket.emit("jackrecstop", "");
      if (msg[0] === "/jackrec/portlist") socket.emit("jackrecportlist", "");
      if (msg[0] === "/jackrec/port") socket.emit("jackrecaddport", msg[1]);
      if (msg[0] === "/jackrec/filelist") socket.emit("jackrecfilelist", "");
      if (msg[0] === "/jackrec/file") socket.emit("jackrecaddfile", msg[1]);
      if (msg[0] === "/jackrec/rectime") socket.emit("jackrectime", msg[1]);
      if (msg[0] === "/jackrec/error") socket.emit("jackrecerr", msg[1]);
    });
    oscClient.send("/touchosc/connect", 16);
    oscClient.send("/jackrec/listports");
    oscClient.send("/jackrec/listfiles");
  });
  socket.on("message", function (obj) {
    oscClient.send(obj);
  });
  socket.on("msg", function (obj) {
    if (obj.value) {
      oscClient.send(obj.path, obj.value);
    } else {
      oscClient.send(obj.path);
    }
  });
});
console.log("Listening on port " + port);
