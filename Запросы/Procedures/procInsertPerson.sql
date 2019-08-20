use fifaDataBase;

--GO
--IF OBJECT_ID('dbo.InsertPerson', 'U') IS NOT NULL  
--	DROP FUNCTION dbo.InsertPerson; 

--GO
--CREATE PROCEDURE InsertPerson(@name nvarchar(50), @passportS int, @passportN int, @phone varchar(11), @prior char)
--AS
--BEGIN
--	INSERT INTO dbo.Persons (pname, ppassportS, ppassportN, pphone, ppriority)
--		VALUES (@name, @passportS, @passportN, @phone, @prior)
--END

EXEC InsertPerson 'Петров Петр Петрович', 2421, 123241, '79031999621', 'C';