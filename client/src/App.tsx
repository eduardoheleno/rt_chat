import React, { useEffect, useRef } from 'react';
import logo from './logo.svg';
import './App.css';

function App() {
  const ws = useRef<WebSocket>();

  useEffect(() => {
    ws.current = new WebSocket("ws://0.0.0.0:8081", "test");
    ws.current.onopen = () => console.log('ws opened');
    ws.current.onclose = () => console.log('ws closed');

    const wsCurrent = ws.current;

    return () => {
      console.log('close');
      wsCurrent.close();
    }
  }, []);

  function sendMessage() {
    ws.current?.send("client sent a message");
  }

  return (
    <div className="App">
      <header className="App-header">
        <img src={logo} className="App-logo" alt="logo" />
        <p>
          Edit <code>src/App.tsx</code> and save to reload.
        </p>
        <button onClick={sendMessage}>
          Send websocket message
        </button>
        <a
          className="App-link"
          href="https://reactjs.org"
          target="_blank"
          rel="noopener noreferrer"
        >
          Learn React
        </a>
      </header>
    </div>
  );
}

export default App;
