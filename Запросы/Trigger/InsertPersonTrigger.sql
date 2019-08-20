use fifaDataBase;

GO
CREATE TRIGGER InsertPersonTrigger
ON PersonsView
INSTEAD OF INSERT
AS 
BEGIN
	DECLARE @name NVARCHAR(30);
	DECLARE @passport NVARCHAR(10);
	DECLARE @phone NVARCHAR(11);
	DECLARE @access CHAR;
	DECLARE @login NVARCHAR(30);
	DECLARE @password NVARCHAR(30);
	DECLARE @pid INT;

	SELECT @name = persName, @passport = passport, @phone = phone, @access = access, @login = persLogin, @password = persPassword FROM inserted;

	INSERT INTO dbo.Persons(pname, ppassport, pphone, ppriority) VALUES(@name, @passport, @phone, @access);
	SELECT @pid = pid FROM dbo.Persons WHERE ppassport LIKE @passport;
	INSERT INTO dbo.Auth(alogin, apassword, pid) VALUES(@login, @password, @pid);
END
