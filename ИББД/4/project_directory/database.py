import os
from sqlalchemy import create_engine
from sqlalchemy.orm import sessionmaker
import logging
from project_directory.models import Base

# Настройка логирования
logging.basicConfig(level=logging.INFO)
logger = logging.getLogger(__name__)

# Конфигурация базы данных с использованием переменных окружения
DATABASE_URL = f'postgresql+psycopg2://{os.getenv("DB_USER")}:{os.getenv("DB_PASSWORD")}@{os.getenv("DB_HOST")}/{os.getenv("DB_NAME")}'

# Создание двигателя для подключения к базе данных
engine = create_engine(DATABASE_URL)

# Создание таблиц, если они еще не существуют
Base.metadata.create_all(bind=engine)

# Настройка сессии
SessionLocal = sessionmaker(autocommit=False, autoflush=False, bind=engine)

# Функция для получения сессии базы данных
def get_db():
    db = SessionLocal()
    try:
        yield db
    finally:
        db.close()
