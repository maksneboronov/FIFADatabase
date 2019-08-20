use fifaDataBase;

GO
CREATE TRIGGER UpdatePersonTrigger
ON PersonsView
INSTEAD OF UPDATE
AS 
BEGIN
	DECLARE @name NVARCHAR(30);
	DECLARE @passport NVARCHAR(10);
	DECLARE @phone NVARCHAR(11);
	DECLARE @access CHAR;
	DECLARE @login NVARCHAR(30);
	DECLARE @password NVARCHAR(30);
	DECLARE @pid INT;

	SELECT @pid = id, @name = persName, @passport = passport, @phone = phone, @access = access, @login = persLogin, @password = persPassword FROM inserted;

	UPDATE dbo.Persons SET pname = @name, ppassport = @passport, pphone = @phone, ppriority = @access WHERE pid = @pid;
	UPDATE dbo.Auth SET alogin = @login, apassword = @password WHERE pid = @pid;
END