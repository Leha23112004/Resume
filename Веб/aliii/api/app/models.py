from sqlalchemy import Column, Integer, String  
from .database import Base  
  
class Student(Base):  
    __tablename__ = "students"  
  
    id = Column(Integer, primary_key=True, index=True)  
    surname = Column(String)  
    name = Column(String)  
    patronymic = Column(String)  
    course = Column(Integer)  
    group = Column(String)  
    faculty = Column(String)