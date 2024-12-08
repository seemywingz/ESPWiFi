import React, { useState, useEffect } from 'react';
import { createTheme, ThemeProvider } from '@mui/material/styles';
import Container from '@mui/material/Container';
import { LinearProgress } from '@mui/material';
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
  const [loading, setLoading] = useState(true);


  useEffect(() => {
    fetch('/config')
      .then((response) => {
        if (!response.ok) {
          throw new Error("Network response was not ok");
        }
        return response.json();
      })
      .then((data) => {
        setConfig(data);
        setLoading(false);
      })
      .catch((error) => {
        console.error('Error loading configuration:', error);
        setLoading(false);
      });
  }, []);

  if (loading) {
    return <LinearProgress color="inherit" />
  }

  const saveConfig = (newConfig) => {
    fetch('/config', {
      method: 'POST',
      headers: { 'Content-Type': 'application/json' },
      body: JSON.stringify(newConfig),
    })
      .then((response) => {
        if (!response.ok) throw new Error('Failed to save configuration');
        return response.json();
      })
      .then((savedConfig) => {
        setConfig(savedConfig);
        console.log('Configuration saved successfully:', savedConfig);
        alert('Configuration saved successfully');
      })
      .catch((error) => {
        console.error('Error saving configuration:', error);
        alert('Failed to save configuration');
      });
  };

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
        <Settings config={config} saveConfig={saveConfig} />
      </Container>
    </ThemeProvider>
  );
}

export default App;
