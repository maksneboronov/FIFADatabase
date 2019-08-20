use fifaDataBase;

Go
CREATE PROCEDURE UpdatePerson(
	@name			NVARCHAR(30),
	@passport		NVARCHAR(10),
	@phone			VARCHAR(11),
	@priority		CHAR(1),
	@login			NVARCHAR(30),
	@password		NVARCHAR(30),
	@pid			INT	)
AS
BEGIN
	UPDATE dbo.Persons SET pname = @name, ppassport = @passport, pphone = @phone, ppriority = @priority WHERE pid = @pid;
	UPDATE dbo.Auth SET alogin = @login, apassword = @password WHERE pid = @pid;
END