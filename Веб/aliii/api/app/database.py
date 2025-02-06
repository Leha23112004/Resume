from sqlalchemy import create_engine  
from sqlalchemy.orm import sessionmaker, declarative_base  
import os  
  
DB_USER = os.getenv("POSTGRES_USER", "user")  
DB_PASSWORD = os.getenv("POSTGRES_PASSWORD", "pass")  
DB_HOST = os.getenv("POSTGRES_HOST", "db")  
DB_PORT = os.getenv("POSTGRES_PORT", "5432")  
DB_NAME = os.getenv("POSTGRES_DB", "studentsdb")  
  
SQLALCHEMY_DATABASE_URL = f"postgresql://{DB_USER}:{DB_PASSWORD}@{DB_HOST}:{DB_PORT}/{DB_NAME}"  
  
engine = create_engine(SQLALCHEMY_DATABASE_URL)  
SessionLocal = sessionmaker(autocommit=False, autoflush=False, bind=engine)  
  
Base = declarative_base()  