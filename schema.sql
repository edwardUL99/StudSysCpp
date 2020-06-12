/*schema here*/

/*make sure its 3NF, go over sulis slides and your project and videos to make sure it is */

CREATE TABLE IF NOT EXISTS lecturers (
	id INTEGER NOT NULL,
	name VARCHAR(255) NOT NULL,
	age INTEGER(3),
	email VARCHAR(255),
	department VARCHAR(100),
	PRIMARY KEY (id),
	UNIQUE KEY (email)
);

CREATE TABLE IF NOT EXISTS courses (
	id CHAR(5),
	type CHAR(5) NOT NULL, /*e.g BSc*/
	name VARCHAR(255) NOT NULL,
	duration INTEGER(1),
	course_leader INTEGER,
	PRIMARY KEY (id),
	FOREIGN KEY (course_leader) REFERENCES lecturers(id)
		ON DELETE RESTRICT
		ON UPDATE CASCADE
);

CREATE TABLE IF NOT EXISTS students (
	id INTEGER NOT NULL,
	name VARCHAR(255) NOT NULL,
	age INTEGER,
	email VARCHAR(255),
	qca FLOAT DEFAULT 0.00, /*Maybe have a procedure or trigger that updates qca after grade is totalled */
	course CHAR(5),
	PRIMARY KEY (id),
	UNIQUE KEY (email),
	FOREIGN KEY (course) REFERENCES courses(id)
		ON DELETE RESTRICT
		ON UPDATE CASCADE
);

CREATE TABLE IF NOT EXISTS modules (
	code CHAR(6) NOT NULL,
	name VARCHAR(100),
	credits INTEGER(1),
	lecturer INTEGER,
	PRIMARY KEY (code),
	FOREIGN KEY (lecturer) REFERENCES lecturers(id)
		ON DELETE RESTRICT
		ON UPDATE CASCADE
);

CREATE TABLE IF NOT EXISTS exams (
	id INTEGER NOT NULL AUTO_INCREMENT,
	module CHAR(6),
	name VARCHAR(100),
	year INTEGER,
	semester INTEGER,
	numberOfQuestions INTEGER,
	weightPerQuestion FLOAT,
	totalWeight FLOAT,
	PRIMARY KEY (id, name),
	UNIQUE KEY (name, year, semester),
	FOREIGN KEY (module) REFERENCES modules(code)
		ON DELETE CASCADE
		ON UPDATE CASCADE 
);

/*You mightn't need this table if its just answer, you could just have it in exam_questions not as foreign key*/
CREATE TABLE IF NOT EXISTS exam_answer_keys (
	answer VARCHAR(50),
	PRIMARY KEY(answer)
);

CREATE TABLE IF NOT EXISTS exam_questions (
	exam INTEGER NOT NULL,
	question VARCHAR(255),
	answer_key VARCHAR(50),
	numberOfAnswers INTEGER,
	PRIMARY KEY (exam, question),
	FOREIGN KEY (exam) REFERENCES exams(id)
		ON DELETE CASCADE 
		ON UPDATE CASCADE,
	FOREIGN KEY (answer_key) REFERENCES exam_answer_keys(answer)
		ON DELETE CASCADE 
		ON UPDATE CASCADE
);

CREATE TABLE IF NOT EXISTS exam_answers (
	exam INTEGER,
	question VARCHAR(255),
	answer VARCHAR(50),
	PRIMARY KEY (exam, question, answer),
	FOREIGN KEY (exam, question) REFERENCES exam_questions(exam, question)
		ON DELETE CASCADE
		ON UPDATE CASCADE
);

CREATE TABLE IF NOT EXISTS exam_grades (
	student INTEGER,
	exam INTEGER,
	grade FLOAT,
	date_recorded DATETIME DEFAULT CURRENT_TIMESTAMP,
	PRIMARY KEY (student, exam),
	FOREIGN KEY (student) REFERENCES students(id)
		ON DELETE CASCADE
		ON UPDATE CASCADE,
	FOREIGN KEY (exam) REFERENCES exams(id)
		ON DELETE CASCADE
		ON UPDATE CASCADE
);

CREATE TABLE IF NOT EXISTS module_grades (
	module CHAR(6),
	student INTEGER,
	mark FLOAT,
	date_recorded DATETIME DEFAULT CURRENT_TIMESTAMP,
	PRIMARY KEY (module, student),
	FOREIGN KEY (student) REFERENCES students(id)
		ON DELETE CASCADE 
		ON UPDATE CASCADE,
	FOREIGN KEY (module) REFERENCES modules(code)
		ON DELETE RESTRICT
		ON UPDATE CASCADE
);

CREATE TABLE IF NOT EXISTS lecturer_accounts (
	id INTEGER,
	pass VARCHAR(16),
	PRIMARY KEY (id),
	FOREIGN KEY (id) REFERENCES lecturers(id)
		ON DELETE CASCADE 
		ON UPDATE CASCADE
);

CREATE TABLE IF NOT EXISTS student_accounts (
	id INTEGER,
	pass VARCHAR(16),
	PRIMARY KEY (id),
	FOREIGN KEY (id) REFERENCES students(id)
		ON DELETE CASCADE 
		ON UPDATE CASCADE 
);

/*other tables below*/
