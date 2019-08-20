use fifaDataBase;

GO
CREATE TRIGGER DeleteMatchOnDeleteTeam
ON dbo.Teams
AFTER DELETE 
AS
BEGIN
	DECLARE @deleteTeamId FLOAT

	SELECT @deleteTeamId = (SELECT tid FROM deleted)

	DELETE FROM dbo.Matches WHERE tid1 = @deleteTeamId OR tid2 = @deleteTeamId;
END