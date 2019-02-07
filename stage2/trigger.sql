CREATE DATABASE university;
SHOW DATABASES;

USE university;
SHOW TABLES;

CREATE TABLE department (
    id INT NOT NULL AUTO_INCREMENT PRIMARY KEY,
    name VARCHAR(128) NOT NULL
);

INSERT INTO department (name) VALUES ("CSE");
INSERT INTO department (name) VALUES ("MnC");
INSERT INTO department (name) VALUES ("ECE");

CREATE TABLE course (
    id CHAR(5) NOT NULL PRIMARY KEY,
    name VARCHAR(128) NOT NULL,
    dept_id INT NOT NULL,
    FOREIGN KEY (dept_id) REFERENCES department (id)
);

INSERT INTO course (id, name, dept_id) VALUES ("CS344", "Databases", 1);
INSERT INTO course (id, name, dept_id) VALUES ("CS345", "Databases Lab", 1);
INSERT INTO course (id, name, dept_id) VALUES ("EE101", "Electrical Science", 3);

CREATE TABLE lecturer (
    id INT NOT NULL AUTO_INCREMENT PRIMARY KEY,
    name VARCHAR(128) NOT NULL,
    dept_id INT NOT NULL,
    FOREIGN KEY (dept_id) REFERENCES department (id) 
);  

INSERT INTO lecturer (name, dept_id) VALUES ("Dr. Singh", 1);
INSERT INTO lecturer (name, dept_id) VALUES ("Dr. Inkulu", 1);
INSERT INTO lecturer (name, dept_id) VALUES ("Dr. Tony Jacob", 3);

CREATE TABLE student (
    id INT NOT NULL AUTO_INCREMENT PRIMARY KEY,
    name VARCHAR(128) NOT NULL,
    dept_id INT NOT NULL,
    FOREIGN KEY (dept_id) REFERENCES department (id)
);

INSERT INTO student (name, dept_id) VALUES ("Nitin Kedia", 1);
INSERT INTO student (name, dept_id) VALUES ("Sahib Khan", 1);
INSERT INTO student (name, dept_id) VALUES ("Pant Rohit Rakesh", 1);
INSERT INTO student (name, dept_id) VALUES ("Ram", 1);

CREATE TABLE student_course (
    student_id INT NOT NULL,
    course_id CHAR(5) NOT NULL,
    PRIMARY KEY (student_id, course_id),
    FOREIGN KEY (student_id) REFERENCES student (id),
    FOREIGN KEY (course_id) REFERENCES course (id)
);

DELIMITER $$
CREATE TRIGGER course_enroll BEFORE INSERT ON student_course
FOR EACH ROW
BEGIN
    SET @course_dept_id := (SELECT dept_id FROM course WHERE id=NEW.course_id LIMIT 1);
    IF (@course_dept_id <=> NULL) THEN
        SIGNAL SQLSTATE '45000'
            SET MESSAGE_TEXT = 'student can not enroll a course that is not offered in any department';        
    END IF;
END$$
DELIMITER ;

INSERT INTO student_course (student_id, course_id) VALUES (1, "XX000");
INSERT INTO student_course (student_id, course_id) VALUES (1, "CS344");
INSERT INTO student_course (student_id, course_id) VALUES (1, "CS345");
INSERT INTO student_course (student_id, course_id) VALUES (2, "CS344");
INSERT INTO student_course (student_id, course_id) VALUES (2, "CS345");
INSERT INTO student_course (student_id, course_id) VALUES (3, "CS345");

CREATE TABLE lecturer_course (
    lecturer_id INT NOT NULL,
    course_id CHAR(5) NOT NULL,
    PRIMARY KEY (lecturer_id, course_id),
    FOREIGN KEY (lecturer_id) REFERENCES lecturer(id),
    FOREIGN KEY (course_id) REFERENCES course(id)
);

DELIMITER $$
CREATE TRIGGER same_dept BEFORE INSERT ON lecturer_course
FOR EACH ROW
BEGIN
    SET @lecturer_dept_id := (SELECT dept_id FROM lecturer WHERE id=NEW.lecturer_id LIMIT 1);
    SET @course_dept_id := (SELECT dept_id FROM course WHERE id=NEW.course_id LIMIT 1);

    IF (@course_dept_id <=> NULL OR @lecturer_dept_id != @course_dept_id) THEN
        SIGNAL SQLSTATE '45000'
            SET MESSAGE_TEXT = 'Both must have same non null dept_id';
    END IF;
END$$
DELIMITER ;

INSERT INTO lecturer_course (lecturer_id, course_id) VALUES (1, "CS344");
INSERT INTO lecturer_course (lecturer_id, course_id) VALUES (3, "CS344");
INSERT INTO lecturer_course (lecturer_id, course_id) VALUES (3, "XX000");
INSERT INTO lecturer_course (lecturer_id, course_id) VALUES (1, "CS345");
INSERT INTO lecturer_course (lecturer_id, course_id) VALUES (3, "EE101");

SELECT name
FROM student
WHERE id IN (
    SELECT student_id FROM student_course WHERE course_id="CS345"
)

SELECT name FROM student WHERE id IN (
    SELECT student_id FROM student_course WHERE course_id IN (
        SELECT course_id FROM lecturer_course WHERE lecturer_id='1'
    )
);

SELECT name FROM course WHERE id IN (
    SELECT course_id FROM student_course WHERE student_id=1
) AND dept_id != (SELECT dept_id FROM student WHERE id=1);

ALTER TABLE student ADD email VARCHAR(128) UNIQUE;
ALTER TABLE student MODIFY email VARCHAR(64); -- does not remove unique
ALTER TABLE student DROP INDEX email;
ALTER TABLE student DROP COLUMN email;


CREATE TABLE teach (
    lecturer_id INT NOT NULL,
    course_id CHAR(5) NOT NULL,
    dept_id INT NOT NULL,
    FOREIGN KEY (dept_id, lecturer_id) REFERENCES lecturer (dept_id, id),
    FOREIGN KEY (dept_id, course_id) REFERENCES course (dept_id, id), 
    PRIMARY KEY (lecturer_id, course_id)
);

--f

SELECT name FROM course WHERE id IN (
    SELECT course_id FROM student_course WHERE student_id=1
)

-- g
SELECT name FROM student WHERE id IN ( 
    SELECT student_id FROM student_course WHERE course_id IN (
        SELECT course_id FROM student_course WHERE student_id=1
    )
);
-- h
    (
    SELECT dept_id FROM student WHERE id=1
)
