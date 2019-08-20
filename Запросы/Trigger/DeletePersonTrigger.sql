use fifaDataBase;

Go
CREATE TRIGGER DeletePersonTrigger
ON PersonsView
INSTEAD OF DELETE
AS 
BEGIN
	DECLARE @pid INT;

	SELECT @pid = id FROM deleted;

	DELETE FROM dbo.Persons WHERE pid = @pid;
END