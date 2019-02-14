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