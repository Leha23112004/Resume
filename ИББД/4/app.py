from flask import Flask, request, jsonify, g, render_template, redirect, url_for
from project_directory.database import SessionLocal
from project_directory.models import Teacher, Course
from sqlalchemy.exc import IntegrityError
import logging

# Настройка логирования
logging.basicConfig(level=logging.INFO)
logger = logging.getLogger(__name__)

app = Flask(__name__, template_folder='project_directory/templates')

# Создание функции для получения сессии работы с БД
def get_db():
    if 'db' not in g:
        g.db = SessionLocal()
    return g.db

@app.teardown_appcontext
def shutdown_session(exception=None):
    db = g.pop('db', None)
    if db is not None:
        db.close()

# Главная страница
@app.route('/')
def index():
    return render_template('index.html', title='Главная страница')

# Страница со списком преподавателей
@app.route('/teachers', methods=['GET'])
def get_teachers():
    db = get_db()
    logger.info("Fetching all teachers")
    teachers = db.query(Teacher).all()
    return render_template('teachers.html', teachers=teachers, title='Список преподавателей')

# Страница для добавления преподавателя
@app.route('/teachers/add', methods=['GET', 'POST'])
def add_teacher():
    db = get_db()
    if request.method == 'POST':
        data = request.form
        # Проверка, существует ли преподаватель с таким email
        existing_teacher = db.query(Teacher).filter_by(teacher_email=data['email']).first()
        if existing_teacher:
            return render_template('add_teacher.html', error='Преподаватель с таким email уже существует', title='Добавить преподавателя')

        # Создаем нового преподавателя
        new_teacher = Teacher(
            teacher_firstname=data['firstname'],
            teacher_lastname=data['lastname'],
            teacher_phone=data['phone'],
            teacher_email=data['email'],
            teacher_specialization=data['specialization']
        )
        db.add(new_teacher)
        try:
            db.commit()
            logger.info(f"Teacher added successfully: {data['firstname']} {data['lastname']}")
            return redirect(url_for('get_teachers'))
        except Exception as e:
            db.rollback()
            logger.error(f"Unexpected error: {str(e)}")
            return render_template('add_teacher.html', error='Ошибка на сервере', title='Добавить преподавателя')

    return render_template('add_teacher.html', title='Добавить преподавателя')

# Удаление преподавателя по ID
@app.route('/teachers/delete/<int:teacher_id>', methods=['POST'])
def delete_teacher(teacher_id):
    db = get_db()
    teacher = db.query(Teacher).filter(Teacher.teacher_id == teacher_id).first()
    if teacher:
        db.delete(teacher)
        db.commit()
        logger.info(f"Teacher deleted successfully: ID {teacher_id}")
        return redirect(url_for('get_teachers'))
    else:
        logger.warning(f"Teacher with ID {teacher_id} not found")
        return redirect(url_for('get_teachers'), code=404)
    
    # Удаление курса по ID
@app.route('/courses/delete/<int:course_id>', methods=['POST'])
def delete_course(course_id):
    db = get_db()
    course = db.query(Course).filter(Course.course_id == course_id).first()
    if course:
        db.delete(course)
        db.commit()
        logger.info(f"Course deleted successfully: ID {course_id}")
        return redirect(url_for('get_courses'))
    else:
        logger.warning(f"Course with ID {course_id} not found")
        return redirect(url_for('get_courses'), code=404)


# Страница со списком курсов
@app.route('/courses', methods=['GET'])
def get_courses():
    db = get_db()
    courses = db.query(Course).all()
    return render_template('courses.html', courses=courses, title='Список курсов')

# Страница для добавления курса
@app.route('/courses/add', methods=['GET', 'POST'])
def add_course():
    db = get_db()
    if request.method == 'POST':
        data = request.form
        new_course = Course(
            course_name=data['course_name'],
            description=data['description'],
            duration=data['duration'],
            cost=data['cost'],
            teacher_id=data['teacher_id']
        )
        db.add(new_course)
        try:
            db.commit()
            logger.info(f"Course added successfully: {data['course_name']}")
            return redirect(url_for('get_courses'))
        except IntegrityError as e:
            db.rollback()
            logger.error(f"IntegrityError: {str(e)}")
            return render_template('add_course.html', error=f'Ошибка при добавлении курса: {str(e)}', title='Добавить курс')
        except Exception as e:
            db.rollback()
            logger.error(f"Unexpected error: {str(e)}")
            return render_template('add_course.html', error=f'Ошибка на сервере: {str(e)}', title='Добавить курс')
    
    return render_template('add_course.html', title='Добавить курс')
if __name__ == '__main__':
    app.run(debug=True, host='0.0.0.0', port=5000)
