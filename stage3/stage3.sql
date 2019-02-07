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

--b
-- Method1 intersection of students in each of given student's course
-- Method2 student_id in enroll who have all course opted by given student

-- Using difference
SELECT DISTINCT name FROM student JOIN enroll AS sx ON id=student_id
WHERE NOT EXISTS (
    (SELECT course_id FROM enroll WHERE student_id = 2 AND course_id NOT IN
    (SELECT sp.course_id FROM enroll as sp WHERE sp.student_id=sx.student_id))
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
        SELECT DISTINCT student_id, lecturer_id FROM enroll JOIN teach ON enroll.course_id = teach.course_id WHERE lecturer_id = 1 OR lecturer_id = 2)
        WHERE A.student_id = id
    );


SELECT name FROM student JOIN enroll ON id=student_id 
    WHERE student_id IN (
        SELECT student_id FROM enroll WHERE course_id IN (
        SELECT course_id FROM teach WHERE lecturer_id=1
    )
    AND student_id NOT IN (
        SELECT student_id FROM enroll WHERE course_id IN (
        SELECT course_id FROM teach WHERE lecturer_id=4
        )
    )
) UNION
SELECT name FROM student JOIN enroll ON id=student_id 
    WHERE student_id IN (
        SELECT student_id FROM enroll WHERE course_id IN (
        SELECT course_id FROM teach WHERE lecturer_id=4
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
