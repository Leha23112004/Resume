import React from 'react';  
import { Table, TableHead, TableBody, TableRow, TableCell, Button } from '@mui/material';  
  
const StudentsTable = ({ students, onEdit, onDelete }) => {  
  return (  
    <Table sx={{ borderRadius:1, overflow:'hidden', border:'1px solid #00e5ff', boxShadow:'0 0 8px #d500f9' }}>  
      <TableHead>  
        <TableRow sx={{ backgroundColor:'#24243e' }}>  
          <TableCell sx={{ fontWeight:600, color:'#00e5ff' }}>ID</TableCell>  
          <TableCell sx={{ fontWeight:600, color:'#00e5ff' }}>Фамилия</TableCell>  
          <TableCell sx={{ fontWeight:600, color:'#00e5ff' }}>Имя</TableCell>  
          <TableCell sx={{ fontWeight:600, color:'#00e5ff' }}>Отчество</TableCell>  
          <TableCell sx={{ fontWeight:600, color:'#00e5ff' }}>Курс</TableCell>  
          <TableCell sx={{ fontWeight:600, color:'#00e5ff' }}>Группа</TableCell>  
          <TableCell sx={{ fontWeight:600, color:'#00e5ff' }}>Факультет</TableCell>  
          <TableCell sx={{ fontWeight:600, color:'#00e5ff' }}>Действия</TableCell>  
        </TableRow>  
      </TableHead>  
      <TableBody>  
        {students.map((s, idx) => (  
          <TableRow key={s.id} sx={{ backgroundColor: idx % 2 === 0 ? '#1c1b29' : '#302b63' }}>  
            <TableCell sx={{ color:'#ffffff' }}>{s.id}</TableCell>  
            <TableCell sx={{ color:'#ffffff' }}>{s.surname}</TableCell>  
            <TableCell sx={{ color:'#ffffff' }}>{s.name}</TableCell>  
            <TableCell sx={{ color:'#ffffff' }}>{s.patronymic}</TableCell>  
            <TableCell sx={{ color:'#ffffff' }}>{s.course}</TableCell>  
            <TableCell sx={{ color:'#ffffff' }}>{s.group}</TableCell>  
            <TableCell sx={{ color:'#ffffff' }}>{s.faculty}</TableCell>  
            <TableCell>  
              <Button variant="contained" color="primary" onClick={() => onEdit(s)} sx={{ mr:1, fontWeight:600 }}>Редактировать</Button>  
              <Button variant="contained" color="error" onClick={() => onDelete(s.id)} sx={{ fontWeight:600 }}>Удалить</Button>  
            </TableCell>  
          </TableRow>  
        ))}  
      </TableBody>  
    </Table>  
  );  
};  
  
export default StudentsTable; 