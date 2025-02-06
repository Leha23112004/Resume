CREATE TABLE IF NOT EXISTS students (  
    id SERIAL PRIMARY KEY,  
    surname VARCHAR(100),  
    name VARCHAR(100),  
    patronymic VARCHAR(100),  
    course INT,  
    "group" VARCHAR(50),  
    faculty VARCHAR(100)  
);  
  