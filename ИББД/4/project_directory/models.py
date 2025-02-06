from sqlalchemy import Column, Integer, String, ForeignKey, Date
from sqlalchemy.orm import relationship
from sqlalchemy.ext.declarative import declarative_base

Base = declarative_base()

# Модель 'Teacher'
class Teacher(Base):
    __tablename__ = 'teachers'

    teacher_id = Column(Integer, primary_key=True, index=True)
    teacher_firstname = Column(String, nullable=False)
    teacher_lastname = Column(String, nullable=False)
    teacher_phone = Column(String, nullable=False)
    teacher_email = Column(String, unique=True, nullable=False)
    teacher_specialization = Column(String, nullable=False)

    courses = relationship("Course", back_populates="teacher")

# Модель 'Course'
class Course(Base):
    __tablename__ = 'courses'

    course_id = Column(Integer, primary_key=True, index=True)  
    course_name = Column(String, nullable=False)
    description = Column(String, nullable=True)
    duration = Column(String, nullable=True)
    cost = Column(Integer, nullable=True)
    teacher_id = Column(Integer, ForeignKey('teachers.teacher_id'))

    teacher = relationship("Teacher", back_populates="courses")

# Модель 'Student'
class Student(Base):
    __tablename__ = 'students'

    student_id = Column(Integer, primary_key=True, index=True)
    student_firstname = Column(String, nullable=False)
    student_lastname = Column(String, nullable=False)
    student_birth_date = Column(Date, nullable=False)
    student_email = Column(String, unique=True, nullable=False)
    student_phone = Column(String, nullable=False)
    student_registration_date = Column(Date, nullable=False)
    student_university = Column(String, nullable=False)
    student_speciality = Column(String, nullable=False)

    enrollments = relationship("Enrollment", back_populates="student")

# Модель 'Enrollment'
class Enrollment(Base):
    __tablename__ = 'enrollments'

    enrollment_id = Column(Integer, primary_key=True, index=True)
    student_id = Column(Integer, ForeignKey('students.student_id'))
    course_id = Column(Integer, ForeignKey('courses.course_id'))
    enrollment_date = Column(Date, nullable=False)

    student = relationship("Student", back_populates="enrollments")
    course = relationship("Course", back_populates="enrollments")

Course.enrollments = relationship("Enrollment", order_by=Enrollment.enrollment_id, back_populates="course")

# Модель 'Payment'
class Payment(Base):
    __tablename__ = 'payments'

    payment_id = Column(Integer, primary_key=True, index=True)
    amount = Column(Integer, nullable=False)
    student_id = Column(Integer, ForeignKey('students.student_id'))
    payment_date = Column(Date, nullable=False)
    payment_method = Column(String, nullable=False)

    student = relationship("Student", back_populates="payments")

Student.payments = relationship("Payment", order_by=Payment.payment_id, back_populates="student")
