import React from 'react';  
import { Box, Button, Select, MenuItem, Typography } from '@mui/material';  
  
const PaginationControls = ({ currentPage, pageSize, onPageChange, onPageSizeChange }) => {  
  const handlePageSizeChange = (e) => {  
    onPageSizeChange(parseInt(e.target.value, 10));  
  };  
  
  const handlePreviousPage = () => {  
    if (currentPage > 0) {  
      onPageChange(currentPage - 1);  
    }  
  };  
  
  const handleNextPage = () => {  
    onPageChange(currentPage + 1);  
  };  
  
  return (  
    <Box display="flex" alignItems="center" gap={2} mt={2} sx={{ color:'#ffffff' }}>  
      <Button variant="outlined" color="primary" onClick={handlePreviousPage} disabled={currentPage === 0} sx={{ fontWeight:600, borderColor:'#00e5ff', color:'#00e5ff' }}>  
        Предыдущая  
      </Button>  
      <Typography sx={{ color:'#00e5ff' }}>Страница: {currentPage + 1}</Typography>  
      <Button variant="outlined" color="primary" onClick={handleNextPage} sx={{ fontWeight:600, borderColor:'#00e5ff', color:'#00e5ff' }}>  
        Следующая  
      </Button>  
      <Select  
        value={pageSize}  
        onChange={handlePageSizeChange}  
        sx={{ backgroundColor:'#1c1b29', borderRadius:1, color:'#d500f9', border:'1px solid #00e5ff' }}  
      >  
        <MenuItem value={5}>5 на страницу</MenuItem>  
        <MenuItem value={10}>10 на страницу</MenuItem>  
        <MenuItem value={20}>20 на страницу</MenuItem>  
      </Select>  
    </Box>  
  );  
};  
  
export default PaginationControls; 