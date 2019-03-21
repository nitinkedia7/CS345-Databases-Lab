-- a.1 (subquery)
SELECT * FROM student WHERE id IN (
    SELECT student_id FROM enroll WHERE course_id NOT IN (
        SELECT course_id FROM enroll WHERE student_id = 4
    )
);

--a.2 (join)
SELECT DISTINCT name FROM student JOIN enroll ON id=student_id WHERE course_id NOT IN (
    SELECT course_id FROM enroll WHERE student_id = 4
);

-- a.3 (not exists)
SELECT DISTINCT name FROM student JOIN enroll AS sx ON id=student_id
WHERE NOT EXISTS (SELECT * FROM enroll WHERE student_id=4 AND course_id=sx.course_id);

--b
-- Method1 intersection of students in each of given student's course
-- Method2 student_id in enroll who have all course opted by given student

-- Using difference
SELECT DISTINCT name FROM student JOIN enroll AS sx ON id=student_id
WHERE NOT EXISTS (
    (SELECT course_id FROM enroll WHERE student_id = 6 AND course_id NOT IN
    (SELECT sp.course_id FROM enroll as sp WHERE sp.student_id=sx.student_id))
);

--b alter
select distinct id,name from student where 
    (select COUNT(course_id) from student_course where stud_id = "2")
    =
    (
        SELECT COUNT(stud_id) from 
        (
            (select course_id from student_course where stud_id = "2") A
            JOIN
            student_course B
            ON A.course_id = B.course_id
        )
        where stud_id =id
    );

--c
-- select student who have attended any of the course offered by both lecturers
SELECT DISTINCT name FROM student JOIN enroll ON id=student_id WHERE course_id IN (
    SELECT course_id FROM teach WHERE lecturer_id=1 OR lecturer_id=2 
);

--d
-- students in one lecturer's course but not in other's
-- A = SELECT enroll.student_id, teach.lecturer_id FROM enroll JOIN teach ON enroll.course_id = teach.course_id WHERE lecturer_id = 1 OR lecturer_id = 2;

-- SELECT student_id FROM A outer WHERE 1 = (
--     SELECT COUNT(student_id) FROM A WHERE student_id = A.student_id;
-- )

SELECT * FROM student
    WHERE 1 = (SELECT COUNT(student_id) FROM (
        SELECT DISTINCT student_id, lecturer_id FROM enroll JOIN teach ON enroll.course_id = teach.course_id WHERE lecturer_id = 1 OR lecturer_id = 2) A
        WHERE A.student_id = id
    );

--check
SELECT *  FROM enroll JOIN teach ON enroll.course_id = teach.course_id WHERE lecturer_id = 1 OR lecturer_id = 2 ORDER BY lecturer_id;

SELECT name FROM student JOIN enroll ON id=student_id 
    WHERE student_id IN (
        SELECT student_id FROM enroll WHERE course_id IN (
        SELECT course_id FROM teach WHERE lecturer_id=1
    )
    AND student_id NOT IN (
        SELECT student_id FROM enroll WHERE course_id IN (
        SELECT course_id FROM teach WHERE lecturer_id=2
        )
    )
) UNION
SELECT name FROM student JOIN enroll ON id=student_id 
    WHERE student_id IN (
        SELECT student_id FROM enroll WHERE course_id IN (
        SELECT course_id FROM teach WHERE lecturer_id=2
    )
    AND student_id NOT IN (
        SELECT student_id FROM enroll WHERE course_id IN (
        SELECT course_id FROM teach WHERE lecturer_id=1
        )
    )
);

--e 
-- average of other course
SELECT name FROM student JOIN enroll ON id=student_id WHERE course_id="CS345" AND score >
(SELECT AVG(score) FROM enroll WHERE course_id="CS344");

-- f
SELECT dept_id, COUNT(*) FROM lecturer GROUP BY dept_id;
-- f with dept names
-- SELECT name, c FROM department JOIN 
-- (SELECT dept_id, COUNT(*) as c FROM lecturer GROUP BY dept_id) A ON id=dept_id;

SELECT department.name, COUNT(*) FROM lecturer JOIN department ON dept_id=department.id GROUP BY dept_id

--g
SELECT * FROM student WHERE NOT EXISTS (
    SELECT * FROM enroll JOIN teach ON enroll.course_id=teach.course_id WHERE student_id=id AND lecturer_id=1
);

--h
-- course wise average marks of students of each department
-- output:  dept_id course_id course_avg
SELECT dept_id, course_id, AVG(score) FROM student JOIN enroll ON id=student_id
GROUP BY dept_id, course_id;

-- check
SELECT * FROM student JOIN enroll ON id=student_id ORDER BY dept_id, course_id;

--i

SELECT * FROM 
    (SELECT name, course_id, AVG(score) as avg_score, dept_id FROM enroll JOIN course ON course_id=id GROUP BY course_id) SX
    JOIN (SELECT dept_id, MAX(avg_score) as max_score FROM (SELECT name, course_id, AVG(score) as avg_score, dept_id FROM enroll JOIN course ON course_id=id GROUP BY course_id) C GROUP BY dept_id) B 
    ON SX.dept_id=B.dept_id AND SX.avg_score=B.max_score;

-- SELECT name, course_id, AVG(score) as max_avg_score, dept_id FROM enroll JOIN course as SX ON course_id=id GROUP BY course_id 
-- HAVING max_avg_score >= ALL (SELECT AVG(score) FROM enroll JOIN (SELECT * FROM course WHERE dept_id=1) as SP ON course_id=id GROUP BY course_id);

-- SELECT dept_id, name, MAX(avg_score) FROM (
-- SELECT name, course_id, AVG(score) as avg_score, dept_id FROM enroll JOIN course ON course_id=id GROUP BY course_id) A GROUP BY dept_id;

--j
SELECT
    CASE WHEN score < 40 THEN 'F'
         WHEN score >= 40 AND score < 70 THEN 'B'
         WHEN score >= 70 THEN 'A'
    END Grade,
    COUNT(*) AS StudentCount
FROM enroll WHERE course_id="MA225" GROUP BY 1;

-- check
SELECT * FROM enroll WHERE course_id="MA225";
