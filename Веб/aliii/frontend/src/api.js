import axios from 'axios';  
  
const API_BASE_URL = 'http://localhost:3000';  
  
export const getStudents = async ({skip=0, limit=10, surnameFilter=''}) => {  
  const params = {};  
  if (skip) params.skip = skip;  
  if (limit) params.limit = limit;  
  if (surnameFilter) params.surname = surnameFilter;  
  
  const response = await axios.get(`${API_BASE_URL}/students`, {params});  
  return response.data;  
};  
  
export const createStudent = async (student) => {  
  const response = await axios.post(`${API_BASE_URL}/students`, student);  
  return response.data;  
};  
  
export const updateStudent = async (id, student) => {  
  const response = await axios.patch(`${API_BASE_URL}/students/${id}`, student);  
  return response.data;  
};  
  
export const deleteStudent = async (id) => {  
  await axios.delete(`${API_BASE_URL}/students/${id}`);  
};  
  