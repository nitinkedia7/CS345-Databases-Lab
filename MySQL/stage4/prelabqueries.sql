--1
-- CREATE INDEX score ON enroll (score);
CREATE INDEX course_score ON enroll (course_id, score);
--list indexes
SHOW INDEX FROM enroll;
--check
-- UPDATE enroll SET score = 80 WHERE student_id=2 and course_id="CS345";
SELECT name FROM student JOIN enroll ON id=student_id AND course_id="CS345" AND score >
(SELECT AVG(score) FROM enroll WHERE course_id="CS344");

--2
CREATE VIEW enrolled_course_name AS SELECT student_id, course.name FROM enroll JOIN course ON course_id=course.id; 
--3
DELIMITER $$
CREATE TRIGGER valid_score_insert BEFORE INSERT ON enroll
FOR EACH ROW
BEGIN
    IF (NEW.score < 0) THEN
        SET NEW.score = 0;
    END IF;
    IF (NEW.score > 100) THEN
        SET NEW.score = 100;
    END IF;
    IF (NEW.score < 40) THEN
        SET NEW.grade = 'F';
    END IF;
    IF (NEW.score >= 40 AND NEW.score < 70) THEN
        SET NEW.grade = 'B';
    END IF;
    IF (NEW.score >= 70) THEN
        SET NEW.grade = 'A';
    END IF;
END$$
DELIMITER ;
DELIMITER $$
CREATE TRIGGER valid_score_update BEFORE UPDATE ON enroll
FOR EACH ROW
BEGIN
    IF (NEW.score < 0) THEN
        SET NEW.score = 0;
    END IF;
    IF (NEW.score > 100) THEN
        SET NEW.score = 100;
    END IF;
END $$
DELIMITER ;

INSERT INTO enroll (student_id, course_id, score) VALUES (1, "MA225", -1), (3, "CS344", 110);

--4
ALTER TABLE enroll ADD attendance INT DEFAULT 75;
UPDATE enroll SET attendance=100 WHERE course_id="CS345";

--5
-- relevant indexes are already present as primary keys
SELECT DISTINCT name FROM student JOIN enroll AS sx ON id=student_id
WHERE NOT EXISTS (
    (SELECT course_id FROM enroll WHERE student_id = 6 AND course_id NOT IN
    (SELECT sp.course_id FROM enroll as sp WHERE sp.student_id=sx.student_id))
);

--6
CREATE INDEX student_name ON student(fname);
SELECT DISTINCT name FROM (SELECT * FROM student WHERE fname=(SELECT fname FROM student WHERE id=6)) sb JOIN enroll AS sx ON id=student_id
WHERE NOT EXISTS (
    (SELECT course_id FROM enroll WHERE student_id = 6 AND course_id NOT IN
    (SELECT sp.course_id FROM enroll as sp WHERE sp.student_id=sx.student_id))
);

--7
-- CREATE VIEW grades AS SELECT
--     student_id,
--     score,
--     CASE WHEN score < 40 THEN 'F'
--          WHEN score >= 40 AND score < 70 THEN 'B'
--          WHEN score >= 70 THEN 'A'
--     END Grade
-- FROM enroll;

DROP VIEW grades;
CREATE VIEW grades AS SELECT * FROM enroll;

SELECT * FROM grades WHERE student_id=1 ;


-- select name from students natural join (SELECT rollno FROM student AS sx ON rollNo=stud_id
-- WHERE NOT EXISTS (
--     (SELECT course_id FROM enrol WHERE stud_id = 2 AND course_id NOT IN
--     (SELECT sp.course_id FROM enrol as sp WHERE sp.stud_id=sx.roolno))))as p where substring(name,1,5)=substring((select name from students),1,5) ;

-- 8
-- INSERT INTO student (name, dept_id, fname) VALUES ("Harshit Kedia", 1, "Harshit");
INSERT INTO grades(student_id, course_id, score) VALUES (6, "EE101", 50);

-- 9
ALTER TABLE enroll ADD grade CHAR(1);
-- modified trigger in 5th question
INSERT INTO enroll(student_id, course_id, score) VALUES (6, "CS345", -10);