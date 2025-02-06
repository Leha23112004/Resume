import React, { useState, useEffect } from 'react';  
import { getStudents, createStudent, updateStudent, deleteStudent } from './api';  
import StudentsTable from './components/StudentsTable';  
import PaginationControls from './components/PaginationControls';  
import StudentForm from './components/StudentForm';  
import { Container, TextField, Button, Typography, Box, AppBar, Toolbar, Paper } from '@mui/material';  
  
const App = () => {  
  const [students, setStudents] = useState([]);  
  const [currentPage, setCurrentPage] = useState(0);  
  const [pageSize, setPageSize] = useState(10);  
  const [surnameFilter, setSurnameFilter] = useState('');  
  const [selectedStudent, setSelectedStudent] = useState(null);  
  const [showForm, setShowForm] = useState(false);  
  
  const loadData = async () => {  
    const skip = currentPage * pageSize;  
    const data = await getStudents({ skip, limit: pageSize, surnameFilter });  
    setStudents(data);  
  };  
  
  useEffect(() => {  
    loadData();  
    // eslint-disable-next-line react-hooks/exhaustive-deps
  }, [currentPage, pageSize, surnameFilter]);  
  
  const handleCreate = () => {  
    setSelectedStudent(null);  
    setShowForm(true);  
  };  
  
  const handleEdit = (student) => {  
    setSelectedStudent(student);  
    setShowForm(true);  
  };  
  
  const handleDelete = async (id) => {  
    await deleteStudent(id);  
    loadData();  
  };  
  
  const handleFormSubmit = async (formData) => {  
    if (selectedStudent) {  
      await updateStudent(selectedStudent.id, formData);  
    } else {  
      await createStudent(formData);  
    }  
    setShowForm(false);  
    loadData();  
  };  
  
  const handleCancel = () => {  
    setShowForm(false);  
  };  
  
  return (  
    <Box>  
      <AppBar position="static" sx={{ mb: 4, background: 'linear-gradient(90deg, #00e5ff 0%, #d500f9 100%)' }}>  
        <Toolbar>  
          <Typography variant="h6" sx={{ flexGrow: 1, fontWeight:700 }}>Студенты ИТМО</Typography>  
        </Toolbar>  
      </AppBar>  
      <Container sx={{ py: 4 }}>  
        <Paper sx={{ p:3, mb:3, borderRadius: 2, boxShadow: '0 0 15px #00e5ff', backgroundColor:'#1c1b29' }}>  
          <Box display="flex" alignItems="center" gap={2} mb={2}>  
            <TextField  
              label="Поиск по фамилии"  
              variant="outlined"  
              value={surnameFilter}  
              onChange={(e) => {  
                setCurrentPage(0);  
                setSurnameFilter(e.target.value);  
              }}  
              InputLabelProps={{ style: { color: '#ffffff' } }}  
              InputProps={{ style: { color: '#00e5ff' } }}  
              sx={{  
                '& .MuiOutlinedInput-root': {  
                  '& fieldset': {  
                    borderColor: '#d500f9'  
                  },  
                  '&:hover fieldset': {  
                    borderColor: '#00e5ff'  
                  }  
                }  
              }}  
            />  
            <Button variant="contained" color="secondary" onClick={handleCreate} sx={{ fontWeight:600 }}>Добавить</Button>  
          </Box>  
  
          {showForm && (  
            <StudentForm  
              initialData={selectedStudent}  
              onSubmit={handleFormSubmit}  
              onCancel={handleCancel}  
            />  
          )}  
  
          <StudentsTable  
            students={students}  
            onEdit={handleEdit}  
            onDelete={handleDelete}  
          />  
  
          <PaginationControls  
            currentPage={currentPage}  
            pageSize={pageSize}  
            onPageChange={setCurrentPage}  
            onPageSizeChange={(size) => { setCurrentPage(0); setPageSize(size); }}  
          />  
        </Paper>  
      </Container>  
    </Box>  
  );  
};  
  
export default App;  