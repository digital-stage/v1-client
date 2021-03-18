const { compile } = require("nexe");
const { hashElement } = require("folder-hash");
const { existsSync, readFileSync, writeFileSync, writeFile } = require("fs");

const options = {
  folders: { exclude: [".*", "node_modules", "test_coverage"] },
  files: { include: ["*.js", "*.json"] },
};

const checkIfBuildNecessary = async () => {
  if (existsSync("webmixer") && existsSync("source.md5")) {
    // Compare hash of source
    const storedHash = readFileSync("source.md5");
    const currentHash = await hashElement(".", options);
    return storedHash !== currentHash;
  }
  return true;
};

checkIfBuildNecessary()
  .then((necessary) => {
    if (necessary) {
      return compile({
        input: "./webmixer.js",
        output: "webmixer",
        build: true,
        mangle: false,
        configure: ["--openssl-no-asm"],
        make: ["-j4"],
        patches: [
          (compiler, next) => {
            compiler.code = () =>
              [compiler.shims.join(""), compiler.startup].join(";");
            return next();
          },
          (compiler, next) => {
            return compiler
              .setFileContentsAsync("lib/_third_party_main.js", compiler.code())
              .then(next);
          },
        ],
      })
        .then(() => hashElement(".", options))
        .then((hash) => writeFileSync("source.md5", hash));
    }
  })
  .then(() => {
    console.log("success");
  });
