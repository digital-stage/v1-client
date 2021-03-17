import {useEffect} from 'react';
import {io} from 'socket.io-client';

const Index = () => {
    useEffect(() => {
        fetch('/api/socket').finally(() => {
            const socket = io()

            socket.on('connect', () => {
                console.log('connect')
                socket.emit('config')
            })

            socket.on('scene', data => {
                console.log('scene', data)
            })

            socket.on('newfader', data => {
                console.log('newfader', data)
            })

            socket.on('updatefader', data => {
                console.log('updatefader', data)
            })

            socket.on('jackrecstart', data => {
                console.log('jackrecstart', data)
            })

            socket.on('jackrecstop', data => {
                console.log('jackrecstop', data)
            })

            socket.on('disconnect', () => {
                console.log('disconnect')
            })
        })
    })

    return <h1>Socket.io</h1>
}
export default Index;