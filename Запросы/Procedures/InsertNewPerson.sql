use fifaDataBase;

Go
CREATE PROCEDURE InsertNewPerson(
	@name			NVARCHAR(30),
	@passport		NVARCHAR(10),
	@phone			VARCHAR(11),
	@priority		CHAR(1),
	@login			NVARCHAR(30),
	@password		NVARCHAR(30),
	@pid			INT	output		)
AS
BEGIN
	INSERT INTO PersonsView(persName, passport, phone, access, persLogin, persPassword) 
		VALUES(@name, @passport, @phone, @priority, @login, @password);

	SELECT @pid = pid FROM dbo.Persons WHERE ppassport LIKE @passport;
END
	