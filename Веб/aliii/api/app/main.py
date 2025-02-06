from fastapi import FastAPI, Depends, HTTPException  
from fastapi.middleware.cors import CORSMiddleware  
from sqlalchemy.orm import Session  
from typing import Optional, List  
from .database import SessionLocal, engine, Base  
from . import models, schemas, crud  
  
Base.metadata.create_all(bind=engine)  
  
app = FastAPI()  
  
# Добавляем CORS middleware  
origins = [  
    "http://localhost",  
    "http://localhost:80"  
]  
  
app.add_middleware(  
    CORSMiddleware,  
    allow_origins=origins,  
    allow_credentials=True,  
    allow_methods=["*"],  
    allow_headers=["*"],  
)  
  
def get_db():  
    db = SessionLocal()  
    try:  
        yield db  
    finally:  
        db.close()  
  
@app.get("/students", response_model=List[schemas.StudentOut])  
def read_students(  
    skip: int = 0,  
    limit: int = 10,  
    surname: Optional[str] = None,  
    db: Session = Depends(get_db)  
):  
    filters = {}  
    if surname:  
        filters["surname"] = surname  
    students = crud.get_students(db, skip=skip, limit=limit, filters=filters)  
    return students  
  
@app.post("/students", response_model=schemas.StudentOut)  
def create_student(student: schemas.StudentCreate, db: Session = Depends(get_db)):  
    return crud.create_student(db, student)  
  
@app.patch("/students/{student_id}", response_model=schemas.StudentOut)  
def update_student(student_id: int, student_data: schemas.StudentUpdate, db: Session = Depends(get_db)):  
    updated = crud.update_student(db, student_id, student_data)  
    if updated is None:  
        raise HTTPException(status_code=404, detail="Student not found")  
    return updated  
  
@app.delete("/students/{student_id}")  
def delete_student(student_id: int, db: Session = Depends(get_db)):  
    deleted = crud.delete_student(db, student_id)  
    if not deleted:  
        raise HTTPException(status_code=404, detail="Student not found")  
    return {"status": "ok"}  