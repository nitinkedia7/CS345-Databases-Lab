CREATE DATABASE final_demo;
SHOW DATABASES;

USE final_demo;
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
INSERT INTO course (id, name, dept_id) VALUES ("XX000", "Deprecated Course", NULL);
INSERT INTO course (id, name, dept_id) VALUES ("MA225", "Probability", 2);

CREATE TABLE lecturer (
    id INT NOT NULL AUTO_INCREMENT PRIMARY KEY,
    name VARCHAR(128) NOT NULL,
    dept_id INT NOT NULL,
    FOREIGN KEY (dept_id) REFERENCES department (id) 
);  

INSERT INTO lecturer (name, dept_id) VALUES ("Dr. Singh", 1);
INSERT INTO lecturer (name, dept_id) VALUES ("Dr. Inkulu", 1);
INSERT INTO lecturer (name, dept_id) VALUES ("Dr. Tony Jacob", 3);
INSERT INTO lecturer (name, dept_id) VALUES ("Dr. Samir Kamal", 2);


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
INSERT INTO student (name, dept_id) VALUES ("Shyam", 3);
INSERT INTO student (name, dept_id) VALUES ("Shivam", 2);

CREATE TABLE enroll (
    student_id INT NOT NULL,
    course_id CHAR(5) NOT NULL,
    FOREIGN KEY (student_id) REFERENCES student (id),
    FOREIGN KEY (course_id) REFERENCES course (id),
    PRIMARY KEY (student_id, course_id)
);
ALTER TABLE enroll ADD score INT NOT NULL;
INSERT INTO enroll (student_id, course_id, score) VALUES (1, "XX000", 5);
INSERT INTO enroll (student_id, course_id, score) VALUES (1, "CS344", 1);
INSERT INTO enroll (student_id, course_id, score) VALUES (1, "CS345", 2);
INSERT INTO enroll (student_id, course_id, score) VALUES (2, "CS344", 3);
INSERT INTO enroll (student_id, course_id, score) VALUES (2, "CS345", 4);
INSERT INTO enroll (student_id, course_id, score) VALUES (3, "CS345", 5);
INSERT INTO enroll (student_id, course_id, score) VALUES (1, "EE101", 6);
-- INSERT INTO enroll (student_id, course_id, score) VALUES (1, "EE101", 6);
INSERT INTO enroll (student_id, course_id, score) VALUES (6, "MA225", 10);


CREATE TABLE teach (
    lecturer_id INT NOT NULL,
    course_id CHAR(5) NOT NULL,
    dept_id INT NOT NULL,
    FOREIGN KEY (dept_id, lecturer_id) REFERENCES lecturer (dept_id, id),
    FOREIGN KEY (dept_id, course_id) REFERENCES course (dept_id, id), 
    PRIMARY KEY (lecturer_id, course_id)
);


INSERT INTO teach (lecturer_id, course_id, dept_id) VALUES (1, "CS344", 1);
INSERT INTO teach (lecturer_id, course_id, dept_id) VALUES (3, "CS344", 1);
INSERT INTO teach (lecturer_id, course_id, dept_id) VALUES (3, "XX000", NULL);
INSERT INTO teach (lecturer_id, course_id, dept_id) VALUES (1, "CS345", 1);
INSERT INTO teach (lecturer_id, course_id, dept_id) VALUES (3, "EE101", 3);
INSERT INTO teach (lecturer_id, course_id, dept_id) VALUES (4, "MA225", 2);



-- a
SELECT name
FROM student
WHERE id IN (
    SELECT student_id FROM enroll WHERE course_id="CS345"
);
-- b
INSERT INTO enroll (student_id, course_id, score) VALUES (4, "EE101", 7);
SELECT name FROM student WHERE id IN (
    SELECT student_id FROM enroll WHERE course_id IN (
        SELECT course_id FROM teach WHERE lecturer_id=1
    )
);
-- c

SELECT name FROM course WHERE id IN (
    SELECT course_id FROM enroll WHERE student_id=1
) AND dept_id != (SELECT dept_id FROM student WHERE id=1);

-- d
INSERT INTO enroll (student_id, course_id, score) VALUES (1, "XX000", 0);

--e
INSERT INTO teach (lecturer_id, course_id, dept_id) VALUES (3, "XX000", 2);

--f

SELECT name FROM course WHERE id IN (
    SELECT course_id FROM enroll WHERE student_id=1
);
-- g
SELECT name FROM student WHERE id IN ( 
    SELECT student_id FROM enroll WHERE course_id IN (
        SELECT course_id FROM enroll WHERE student_id=1
    )
);

SELECT name FROM student WHERE id IN ( 
    SELECT student_id FROM enroll WHERE course_id IN (
        SELECT course_id FROM enroll WHERE student_id=4
    )
);

-- h
INSERT INTO enroll (student_id, course_id, score) VALUES (5, "CS344", 7);
INSERT INTO enroll (student_id, course_id, score) VALUES (5, "CS345", 6);
-- that course exists in given dept and foreign students should also be present 
-- SELECT name FROM course WHERE id IN (
--     SELECT course_id FROM enroll WHERE (
--         SELECT id FROM course WHERE dept_id=1;
--     ) AND enroll.dept_id != 1;
-- );
SELECT name FROM course WHERE id IN (
    SELECT course_id FROM enroll WHERE student_id IN (
        SELECT id FROM student WHERE dept_id!=1
    )
) AND dept_id=1;

SELECT name FROM course WHERE id IN (
    SELECT course_id FROM enroll WHERE student_id IN (
        SELECT id FROM student WHERE dept_id!=3
    )
) AND dept_id=3;
-- i
SELECT name FROM student WHERE id IN (
    SELECT student_id FROM enroll WHERE course_id="CS344" AND score > 1 AND score < 10
);

-- j
INSERT INTO teach (lecturer_id, course_id, dept_id) VALUES (1, "EE101", 3);



INSERT INTO teach (lecturer_id, course_id, dept_id) VALUES (2, "CS344", 1);