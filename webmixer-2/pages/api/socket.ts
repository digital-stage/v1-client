import {Server} from 'socket.io';

const osc = require('node-osc');

const ioHandler = (req, res) => {
    if (!res.socket.server.io) {
        console.log('*First use, starting socket.io')

        const oscServer = new osc.Server(9000, '0.0.0.0');
        const oscClient = new osc.Client('localhost', 9871);

        const io = new Server(res.socket.server)

        io.on('connection', socket => {
            socket.on('config', () => {
                oscClient.send('/status', `${socket.id} connected`);
                oscServer.on('message', async (msg) => {
                    if (msg[0] === '/touchosc/scene') {
                        socket.emit('scene', 'scene');
                    }
                    if (msg[0].startsWith('/touchosc/label') && (!msg[0].endsWith('/color')) && (msg[1].length > 1)) {
                        socket.emit('newfader', msg[0].substr(15), msg[1]);
                    }
                    if (msg[0].startsWith('/touchosc/fader') && (!msg[0].endsWith('/color'))) {
                        socket.emit('updatefader', msg[0], msg[1]);
                    }
                    if (msg[0].startsWith('/touchosc/level')) {
                        socket.emit('updatefader', msg[0], msg[1]);
                    }
                    if (msg[0] === '/jackrec/start')
                        socket.emit('jackrecstart', '');
                    if (msg[0] === '/jackrec/stop')
                        socket.emit('jackrecstop', '');
                    if (msg[0] === '/jackrec/portlist')
                        socket.emit('jackrecportlist', '');
                    if (msg[0] === '/jackrec/port')
                        socket.emit('jackrecaddport', msg[1]);
                    if (msg[0] === '/jackrec/filelist')
                        socket.emit('jackrecfilelist', '');
                    if (msg[0] === '/jackrec/file')
                        socket.emit('jackrecaddfile', msg[1]);
                    if (msg[0] === '/jackrec/rectime')
                        socket.emit('jackrectime', msg[1]);
                    if (msg[0] === '/jackrec/error')
                        socket.emit('jackrecerr', msg[1]);

                });
                oscClient.send('/touchosc/connect', 16);
                oscClient.send('/jackrec/listports');
                oscClient.send('/jackrec/listfiles');
            });
            socket.on('message', (obj) => {
                oscClient.send(obj);
            });
            socket.on('msg', (obj) => {
                if (obj.value) {
                    oscClient.send(obj.path, obj.value);
                } else {
                    oscClient.send(obj.path);
                }
            });
        })

        res.socket.server.io = io
    } else {
        console.log('socket.io already running')
    }
    res.end()
}

export const config = {
    api: {
        bodyParser: false
    }
}

export default ioHandler