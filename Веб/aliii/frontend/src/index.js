import React from 'react';  
import { createRoot } from 'react-dom/client';  
import App from './App';  
import { createTheme, ThemeProvider } from '@mui/material/styles';  
import { CssBaseline } from '@mui/material';  
  
const theme = createTheme({  
  palette: {  
    mode: 'dark',  
    primary: { main: '#00e5ff' },  
    secondary: { main: '#d500f9' },  
    background: { default: '#0f0c29', paper: '#1c1b29' },  
    text: { primary: '#ffffff' }  
  },  
  typography: {  
    fontFamily: 'Montserrat, sans-serif',  
    h4: { fontWeight: 700 },  
    h6: { fontWeight: 600 },  
    body1: { fontSize: '1rem' }  
  }  
});  
  
const container = document.getElementById('root');  
const root = createRoot(container);  
root.render(  
  <ThemeProvider theme={theme}>  
    <CssBaseline />  
    <App />  
  </ThemeProvider>  
);  