CREATE DATABASE stage1;
SHOW DATABASES;

USE university;
SHOW tables; 

CREATE TABLE department (
    id INT NOT NULL PRIMARY KEY,
    name VARCHAR(128) NOT NULL
);

INSERT INTO `department` (`id`, `name`) VALUES (1, "CSE");
INSERT INTO `department` (`id`, `name`) VALUES (2, "MnC");
INSERT INTO `department` (`id`, `name`) VALUES (3, "ECE");

CREATE TABLE course (
    id VARCHAR(5) NOT NULL PRIMARY KEY,
    name VARCHAR(128) NOT NULL,
    dept_id INT NOT NULL,
    FOREIGN KEY (dept_id) REFERENCES department (id)
);
INSERT INTO course (id, name, dept_id) VALUES ("CS344", "Databases", 1);
INSERT INTO course (id, name, dept_id) VALUES ("CS345", "Databases Lab", 1);
INSERT INTO course (id, name, dept_id) VALUES ("EE101", "Electrical Science", 3);


CREATE TABLE lecturer (
    id INT NOT NULL PRIMARY KEY,
    name VARCHAR(128) NOT NULL,
    dept_id INT NOT NULL,
    FOREIGN KEY (dept_id) REFERENCES department (id) 
);  

INSERT INTO lecturer (id, name, dept_id) VALUES (1, "Dr. Inkulu", 1);
INSERT INTO lecturer (id, name, dept_id) VALUES (2, "Dr. Karfa", 1);
INSERT INTO lecturer (id, name, dept_id) VALUES (3, "Dr. Tony Jacob", 3);


CREATE TABLE module (
    id INT NOT NULL PRIMARY KEY,
    name VARCHAR(128) NOT NULL,
    prof_id INT NOT NULL,
    FOREIGN KEY (prof_id) REFERENCES lecturer (id)
);

INSERT INTO module (id, name, prof_id) VALUES (1, "SQL", 1);
INSERT INTO module (id, name, prof_id) VALUES (2, "DP", 1);
INSERT INTO module (id, name, prof_id) VALUES (3, "boolean", 3);
INSERT INTO module (id, name, prof_id) VALUES (4, "RDBMS", 1);


CREATE TABLE student (
    id INT NOT NULL PRIMARY KEY,
    name VARCHAR(128) NOT NULL
);

INSERT INTO student (id, name) VALUES (1, "Nitin Kedia");
INSERT INTO student (id, name) VALUES (2, "Sahib Khan");
INSERT INTO student (id, name) VALUES (3, "Pant Rohit Rakesh");

CREATE TABLE student_module (
    student_id INT NOT NULL,
    module_id INT NOT NULL,
    PRIMARY KEY (student_id, module_id),
    FOREIGN KEY (student_id) REFERENCES student (id),
    FOREIGN KEY (module_id) REFERENCES module (id)
);

INSERT INTO student_module (student_id, module_id) VALUES (1, 1);
INSERT INTO student_module (student_id, module_id) VALUES (1, 2);
INSERT INTO student_module (student_id, module_id) VALUES (2, 1);
INSERT INTO student_module (student_id, module_id) VALUES (2, 2);

CREATE TABLE course_module (
    course_id CHAR(5) NOT NULL,
    module_id INT NOT NULL,
    PRIMARY KEY (course_id, module_id),
    FOREIGN KEY (course_id) REFERENCES course (id),
    FOREIGN KEY (module_id) REFERENCES module (id)
);

INSERT INTO course_module (course_id, module_id) VALUEs ("CS344", 1);
INSERT INTO course_module (course_id, module_id) VALUES ("CS344", 4);
INSERT INTO course_module (course_id, module_id) VALUES ("CS345", 4);

CREATE TABLE lecturer_student (
    lecturer_id INT NOT NULL,
    student_id INT NOT NULL,
    PRIMARY KEY (lecturer_id, student_id),
    FOREIGN KEY (lecturer_id) REFERENCES lecturer (id),
    FOREIGN KEY (student_id) REFERENCES student (id)
);

INSERT INTO lecturer_student (lecturer_id, student_id) VALUES (1, 1);
INSERT INTO lecturer_student (lecturer_id, student_id) VALUES (1, 2);
INSERT INTO lecturer_student (lecturer_id, student_id) VALUES (1, 3);
INSERT INTO lecturer_student (lecturer_id, student_id) VALUES (3, 2);

CREATE TABLE student_course (
    student_id INT NOT NULL,
    course_id CHAR(5) NOT NULL,
    PRIMARY KEY (student_id, course_id),
    FOREIGN KEY (student_id) REFERENCES student (id),
    FOREIGN KEY (course_id) REFERENCES course (id)
);

INSERT INTO student_course (student_id, course_id) VALUES (1, "CS344");
INSERT INTO student_course (student_id, course_id) VALUES (1, "CS345");
INSERT INTO student_course (student_id, course_id) VALUES (2, "CS344");
INSERT INTO student_course (student_id, course_id) VALUES (2, "CS345");

ALTER TABLE student ADD email VARCHAR(128) DEFAULT "abc@iitg.ac.in";
ALTER TABLE student ADD gender CHAR(1);