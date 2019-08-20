USE fifaDataBase;

GO
CREATE PROCEDURE IsUsedMan
(
	@date	DATE,
	@pid	INT,
	@count	INT output
)
AS
BEGIN
	SELECT @count = COUNT(*)
	FROM dbo.Matches as m INNER JOIN dbo.WorkingPeople as w 
	ON m.mid = w.mid 
	WHERE mdate = @date AND w.pid = @pid;
END;