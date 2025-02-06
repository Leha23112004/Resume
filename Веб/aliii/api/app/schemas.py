from pydantic import BaseModel, Field  
from typing import Optional  
  
class StudentBase(BaseModel):  
    surname: str  
    name: str  
    patronymic: str  
    course: int  
    group: str  
    faculty: str  
  
class StudentCreate(StudentBase):  
    pass  
  
class StudentUpdate(BaseModel):  
    surname: Optional[str]  
    name: Optional[str]  
    patronymic: Optional[str]  
    course: Optional[int]  
    group: Optional[str]  
    faculty: Optional[str]  
  
class StudentOut(StudentBase):  
    id: int  
    class Config:  
        from_attributes = True  
  