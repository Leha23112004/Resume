import React, { useState, useEffect } from 'react';  
import { Box, TextField, Button, Typography } from '@mui/material';  
  
const StudentForm = ({ initialData, onSubmit, onCancel }) => {  
  const [formData, setFormData] = useState({  
    surname: '',  
    name: '',  
    patronymic: '',  
    course: 1,  
    group: '',  
    faculty: ''  
  });  
  
  useEffect(() => {  
    if (initialData) {  
      setFormData(initialData);  
    }  
  }, [initialData]);  
  
  const handleChange = (e) => {  
    const { name, value } = e.target;  
    setFormData((prev) => ({ ...prev, [name]: value }));  
  };  
  
  const handleSubmit = (e) => {  
    e.preventDefault();  
    onSubmit(formData);  
  };  
  
  return (  
    <Box component="form" onSubmit={handleSubmit} sx={{  
      border: '2px solid #d500f9',  
      p: 2,  
      mb: 2,  
      borderRadius: 2,  
      backgroundColor: '#1c1b29',  
      boxShadow: '0 0 10px #00e5ff'  
    }}>  
      <Typography variant="h6" mb={2} sx={{ fontWeight:700, color:'#d500f9' }}>Форма студента</Typography>  
      <Box display="flex" flexDirection="column" gap={2}>  
        <TextField label="Фамилия" name="surname" value={formData.surname} onChange={handleChange} required  
          InputLabelProps={{ style: { color: '#ffffff' } }}  
          InputProps={{ style: { color: '#00e5ff' } }} />  
        <TextField label="Имя" name="name" value={formData.name} onChange={handleChange} required  
          InputLabelProps={{ style: { color: '#ffffff' } }}  
          InputProps={{ style: { color: '#00e5ff' } }} />  
        <TextField label="Отчество" name="patronymic" value={formData.patronymic} onChange={handleChange} required  
          InputLabelProps={{ style: { color: '#ffffff' } }}  
          InputProps={{ style: { color: '#00e5ff' } }} />  
        <TextField type="number" label="Курс" name="course" value={formData.course} onChange={handleChange} required  
          InputLabelProps={{ style: { color: '#ffffff' } }}  
          InputProps={{ style: { color: '#00e5ff' } }} />  
        <TextField label="Группа" name="group" value={formData.group} onChange={handleChange} required  
          InputLabelProps={{ style: { color: '#ffffff' } }}  
          InputProps={{ style: { color: '#00e5ff' } }} />  
        <TextField label="Факультет" name="faculty" value={formData.faculty} onChange={handleChange} required  
          InputLabelProps={{ style: { color: '#ffffff' } }}  
          InputProps={{ style: { color: '#00e5ff' } }} />  
      </Box>  
      <Box mt={2} display="flex" gap={2}>  
        <Button type="submit" variant="contained" color="secondary" sx={{ fontWeight:600 }}>Сохранить</Button>  
        <Button type="button" variant="outlined" color="primary" onClick={onCancel} sx={{ fontWeight:600, borderColor:'#00e5ff', color:'#00e5ff' }}>Отмена</Button>  
      </Box>  
    </Box>  
  );  
};  
  
export default StudentForm;  