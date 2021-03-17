import * as fs from "fs";

import {homedir as getHomeDir} from "os";

const homeDir = getHomeDir();

const recordHandler = (req, res) => {
    // download from local directory:
    if (fs.existsSync(`.${req.url}`)) {
        const data = fs.readFileSync(`.${req.url}`);
        res.writeHead(200);
        res.end(data);
        return;
    }
    // check in home directory:
    if (fs.existsSync(homeDir + req.url)) {
        const data = fs.readFileSync(homeDir + req.url);
        res.writeHead(200);
        res.end(data);
        return;
    }
}