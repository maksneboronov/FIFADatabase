use fifaDataBase;

GO
CREATE TRIGGER DeleteMatchOnUpdateGroupTeam
ON dbo.Teams
AFTER UPDATE
AS IF UPDATE(tgroup)
BEGIN
	DECLARE @deleteTeamGroup VARCHAR
	DECLARE @insertTeamGroup VARCHAR

	SELECT @deleteTeamGroup = (SELECT tgroup FROM deleted)
	SELECT @insertTeamGroup = (SELECT tgroup FROM inserted)

	IF (@deleteTeamGroup NOT LIKE @insertTeamGroup)
	BEGIN
		DECLARE @insertTeamId INT
		SELECT @insertTeamId = (SELECT tid FROM deleted)

		DELETE FROM dbo.Matches WHERE tid1 = @insertTeamId OR tid2 = @insertTeamId;
	END
END