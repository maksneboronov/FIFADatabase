USE fifaDataBase;

GO
CREATE PROCEDURE IsUsedTeam
(
	@tid	INT,
	@date	DATE,
	@count	INT output
)
AS
BEGIN
	SELECT @count = COUNT (*) FROM dbo.Matches WHERE (tid1 = @tid OR tid2 = @tid) AND mdate = @date;
END;