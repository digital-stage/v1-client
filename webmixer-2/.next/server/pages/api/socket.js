module.exports =
/******/ (function(modules) { // webpackBootstrap
/******/ 	// The module cache
/******/ 	var installedModules = require('../../ssr-module-cache.js');
/******/
/******/ 	// The require function
/******/ 	function __webpack_require__(moduleId) {
/******/
/******/ 		// Check if module is in cache
/******/ 		if(installedModules[moduleId]) {
/******/ 			return installedModules[moduleId].exports;
/******/ 		}
/******/ 		// Create a new module (and put it into the cache)
/******/ 		var module = installedModules[moduleId] = {
/******/ 			i: moduleId,
/******/ 			l: false,
/******/ 			exports: {}
/******/ 		};
/******/
/******/ 		// Execute the module function
/******/ 		var threw = true;
/******/ 		try {
/******/ 			modules[moduleId].call(module.exports, module, module.exports, __webpack_require__);
/******/ 			threw = false;
/******/ 		} finally {
/******/ 			if(threw) delete installedModules[moduleId];
/******/ 		}
/******/
/******/ 		// Flag the module as loaded
/******/ 		module.l = true;
/******/
/******/ 		// Return the exports of the module
/******/ 		return module.exports;
/******/ 	}
/******/
/******/
/******/ 	// expose the modules object (__webpack_modules__)
/******/ 	__webpack_require__.m = modules;
/******/
/******/ 	// expose the module cache
/******/ 	__webpack_require__.c = installedModules;
/******/
/******/ 	// define getter function for harmony exports
/******/ 	__webpack_require__.d = function(exports, name, getter) {
/******/ 		if(!__webpack_require__.o(exports, name)) {
/******/ 			Object.defineProperty(exports, name, { enumerable: true, get: getter });
/******/ 		}
/******/ 	};
/******/
/******/ 	// define __esModule on exports
/******/ 	__webpack_require__.r = function(exports) {
/******/ 		if(typeof Symbol !== 'undefined' && Symbol.toStringTag) {
/******/ 			Object.defineProperty(exports, Symbol.toStringTag, { value: 'Module' });
/******/ 		}
/******/ 		Object.defineProperty(exports, '__esModule', { value: true });
/******/ 	};
/******/
/******/ 	// create a fake namespace object
/******/ 	// mode & 1: value is a module id, require it
/******/ 	// mode & 2: merge all properties of value into the ns
/******/ 	// mode & 4: return value when already ns object
/******/ 	// mode & 8|1: behave like require
/******/ 	__webpack_require__.t = function(value, mode) {
/******/ 		if(mode & 1) value = __webpack_require__(value);
/******/ 		if(mode & 8) return value;
/******/ 		if((mode & 4) && typeof value === 'object' && value && value.__esModule) return value;
/******/ 		var ns = Object.create(null);
/******/ 		__webpack_require__.r(ns);
/******/ 		Object.defineProperty(ns, 'default', { enumerable: true, value: value });
/******/ 		if(mode & 2 && typeof value != 'string') for(var key in value) __webpack_require__.d(ns, key, function(key) { return value[key]; }.bind(null, key));
/******/ 		return ns;
/******/ 	};
/******/
/******/ 	// getDefaultExport function for compatibility with non-harmony modules
/******/ 	__webpack_require__.n = function(module) {
/******/ 		var getter = module && module.__esModule ?
/******/ 			function getDefault() { return module['default']; } :
/******/ 			function getModuleExports() { return module; };
/******/ 		__webpack_require__.d(getter, 'a', getter);
/******/ 		return getter;
/******/ 	};
/******/
/******/ 	// Object.prototype.hasOwnProperty.call
/******/ 	__webpack_require__.o = function(object, property) { return Object.prototype.hasOwnProperty.call(object, property); };
/******/
/******/ 	// __webpack_public_path__
/******/ 	__webpack_require__.p = "";
/******/
/******/
/******/ 	// Load entry module and return exports
/******/ 	return __webpack_require__(__webpack_require__.s = 1);
/******/ })
/************************************************************************/
/******/ ({

/***/ 1:
/***/ (function(module, exports, __webpack_require__) {

module.exports = __webpack_require__("WkxV");


/***/ }),

/***/ "F+p8":
/***/ (function(module, exports) {

module.exports = require("socket.io");

/***/ }),

/***/ "J5XG":
/***/ (function(module, exports) {

module.exports = require("node-osc");

/***/ }),

/***/ "WkxV":
/***/ (function(module, __webpack_exports__, __webpack_require__) {

"use strict";
__webpack_require__.r(__webpack_exports__);
/* harmony export (binding) */ __webpack_require__.d(__webpack_exports__, "config", function() { return config; });
/* harmony import */ var socket_io__WEBPACK_IMPORTED_MODULE_0__ = __webpack_require__("F+p8");
/* harmony import */ var socket_io__WEBPACK_IMPORTED_MODULE_0___default = /*#__PURE__*/__webpack_require__.n(socket_io__WEBPACK_IMPORTED_MODULE_0__);


const osc = __webpack_require__("J5XG");

const ioHandler = (req, res) => {
  if (!res.socket.server.io) {
    console.log('*First use, starting socket.io');
    const oscServer = new osc.Server(9000, '0.0.0.0');
    const oscClient = new osc.Client('localhost', 9871);
    const io = new socket_io__WEBPACK_IMPORTED_MODULE_0__["Server"](res.socket.server);
    io.on('connection', socket => {
      socket.on('config', () => {
        oscClient.send('/status', `${socket.id} connected`);
        oscServer.on('message', async msg => {
          if (msg[0] === '/touchosc/scene') {
            socket.emit('scene', 'scene');
          }

          if (msg[0].startsWith('/touchosc/label') && !msg[0].endsWith('/color') && msg[1].length > 1) {
            socket.emit('newfader', msg[0].substr(15), msg[1]);
          }

          if (msg[0].startsWith('/touchosc/fader') && !msg[0].endsWith('/color')) {
            socket.emit('updatefader', msg[0], msg[1]);
          }

          if (msg[0].startsWith('/touchosc/level')) {
            socket.emit('updatefader', msg[0], msg[1]);
          }

          if (msg[0] === '/jackrec/start') socket.emit('jackrecstart', '');
          if (msg[0] === '/jackrec/stop') socket.emit('jackrecstop', '');
          if (msg[0] === '/jackrec/portlist') socket.emit('jackrecportlist', '');
          if (msg[0] === '/jackrec/port') socket.emit('jackrecaddport', msg[1]);
          if (msg[0] === '/jackrec/filelist') socket.emit('jackrecfilelist', '');
          if (msg[0] === '/jackrec/file') socket.emit('jackrecaddfile', msg[1]);
          if (msg[0] === '/jackrec/rectime') socket.emit('jackrectime', msg[1]);
          if (msg[0] === '/jackrec/error') socket.emit('jackrecerr', msg[1]);
        });
        oscClient.send('/touchosc/connect', 16);
        oscClient.send('/jackrec/listports');
        oscClient.send('/jackrec/listfiles');
      });
      socket.on('message', obj => {
        oscClient.send(obj);
      });
      socket.on('msg', obj => {
        if (obj.value) {
          oscClient.send(obj.path, obj.value);
        } else {
          oscClient.send(obj.path);
        }
      });
    });
    res.socket.server.io = io;
  } else {
    console.log('socket.io already running');
  }

  res.end();
};

const config = {
  api: {
    bodyParser: false
  }
};
/* harmony default export */ __webpack_exports__["default"] = (ioHandler);

/***/ })

/******/ });