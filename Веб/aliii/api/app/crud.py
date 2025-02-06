from sqlalchemy.orm import Session  
from . import models, schemas  
  
def get_students(db: Session, skip: int = 0, limit: int = 10, filters: dict = {}):  
    query = db.query(models.Student)  
    if "surname" in filters:  
        query = query.filter(models.Student.surname.ilike(f"%{filters['surname']}%"))  
    return query.offset(skip).limit(limit).all()  
  
def create_student(db: Session, student: schemas.StudentCreate):  
    db_student = models.Student(**student.dict())  
    db.add(db_student)  
    db.commit()  
    db.refresh(db_student)  
    return db_student  
  
def delete_student(db: Session, student_id: int):  
    student = db.query(models.Student).filter(models.Student.id == student_id).first()  
    if student:  
        db.delete(student)  
        db.commit()  
        return True  
    return False  
  
def update_student(db: Session, student_id: int, student_data: schemas.StudentUpdate):  
    student = db.query(models.Student).filter(models.Student.id == student_id).first()  
    if student:  
        for key, value in student_data.dict(exclude_unset=True).items():  
            setattr(student, key, value)  
        db.commit()  
        db.refresh(student)  
        return student  
    return None  
  