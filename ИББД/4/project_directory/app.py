from flask import Flask, request, jsonify, g
from database import get_db
from models import Teacher, Course
from sqlalchemy.exc import IntegrityError
import logging

# Настройка логирования
logging.basicConfig(level=logging.INFO)
logger = logging.getLogger(__name__)

app = Flask(__name__)

# Создание функции для получения сессии работы с БД
@app.teardown_appcontext
def shutdown_session(exception=None):
    db = g.pop('db', None)
    if db is not None:
        db.close()

@app.route('/teachers', methods=['GET'])
def get_teachers():
    db = next(get_db())
    logger.info("Fetching all teachers")
    teachers = db.query(Teacher).all()
    return jsonify([{
        'teacher_id': teacher.teacher_id,
        'firstname': teacher.teacher_firstname,
        'lastname': teacher.teacher_lastname,
        'email': teacher.teacher_email,
        'specialization': teacher.teacher_specialization
    } for teacher in teachers])

@app.route('/teachers', methods=['POST'])
def add_teacher():
    db = get_db()
    data = request.get_json()
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
        return jsonify({'message': 'Teacher added successfully'}), 201
    except IntegrityError:
        db.rollback()
        logger.error("Error: Teacher with the same email already exists")
        return jsonify({'message': 'Error: Teacher with the same email already exists'}), 400
    except Exception as e:
        db.rollback()
        logger.exception("Unexpected error occurred")
        return jsonify({'message': 'Internal server error'}), 500

@app.route('/teachers/<int:id>', methods=['DELETE'])
def delete_teacher(id):
    db = get_db()
    teacher = db.query(Teacher).get(id)
    if teacher:
        db.delete(teacher)
        db.commit()
        logger.info(f"Teacher deleted successfully: ID {id}")
        return jsonify({'message': 'Teacher deleted successfully'}), 200
    logger.warning(f"Teacher not found: ID {id}")
    return jsonify({'message': 'Teacher not found'}), 404

@app.route('/teachers/<int:id>', methods=['PUT'])
def update_teacher(id):
    db = get_db()
    teacher = db.query(Teacher).get(id)
    if not teacher:
        logger.warning(f"Teacher not found: ID {id}")
        return jsonify({'message': 'Teacher not found'}), 404

    data = request.get_json()
    teacher.teacher_firstname = data.get('firstname', teacher.teacher_firstname)
    teacher.teacher_lastname = data.get('lastname', teacher.teacher_lastname)
    teacher.teacher_phone = data.get('phone', teacher.teacher_phone)
    teacher.teacher_email = data.get('email', teacher.teacher_email)
    teacher.teacher_specialization = data.get('specialization', teacher.teacher_specialization)

    try:
        db.commit()
        logger.info(f"Teacher updated successfully: ID {id}")
        return jsonify({'message': 'Teacher updated successfully'}), 200
    except IntegrityError:
        db.rollback()
        logger.error("Error: Teacher with the same email already exists")
        return jsonify({'message': 'Error: Teacher with the same email already exists'}), 400
    except Exception as e:
        db.rollback()
        logger.exception("Unexpected error occurred")
        return jsonify({'message': 'Internal server error'}), 500

if __name__ == '__main__':
    app.run(debug=True, host='0.0.0.0', port=5000)
