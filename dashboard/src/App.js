import React, { useState, useEffect } from 'react';
import { createTheme, ThemeProvider } from '@mui/material/styles';
import Container from '@mui/material/Container';
import Pins from './pins/Pins';
import Settings from './Settings';

// Define the theme
const theme = createTheme({
  typography: {
    fontFamily: [
      '-apple-system',
      'BlinkMacSystemFont',
      '"Segoe UI"',
      'Roboto Slab',
      '"Helvetica Neue"',
      'Arial',
      'sans-serif',
      '"Apple Color Emoji"',
      '"Segoe UI Emoji"',
      '"Segoe UI Symbol"',
    ].join(','),
  },
  palette: {
    mode: 'dark',
    primary: {
      main: "#38ffb9",
    },
    secondary: {
      main: "#333",
    },
    error: {
      main: "#ff3838",
    },
  },
});

function App() {
  const [config, setConfig] = useState(null);

  const hostname = process.env.REACT_APP_API_HOST || "localhost";
  const port = process.env.REACT_APP_API_PORT || 80;

  useEffect(() => {

    fetch(`http://${hostname}:${port}/config`)
      .then((response) => {
        if (!response.ok) {
          throw new Error("Network response was not ok");
        }
        return response.json();
      })
      .then((data) => {
        setConfig(data);
        console.log("Config:", data);
      })
      .catch((error) => console.error('Error loading configuration:', error));
  }, [port]);

  return (
    <ThemeProvider theme={theme}>
      <Container
        sx={{
          fontFamily: 'Roboto Slab',
          backgroundColor: 'secondary.light',
          color: 'primary.main',
          fontSize: '3em',
          height: '9vh',
          zIndex: 1000,
          textAlign: 'center',
          display: 'flex',
          alignItems: 'center',
          justifyContent: 'center',
          minWidth: '100%',
        }}
      >ESPWiFi</Container>
      <Container>
        <Settings config={config} />
      </Container>
    </ThemeProvider>
  );
}

export default App;
