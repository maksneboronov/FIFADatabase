use fifaDataBase;

GO

CREATE FUNCTION GetWorkingPeoplesOnMatch (@mid INT)
RETURNS TABLE
AS RETURN
(
	SELECT p.pid, p.pname, p.pphone, p.ppriority 
	FROM dbo.WorkingPeople as w INNER JOIN dbo.Persons as p 
	ON w.pid = p.pid 
	WHERE w.mid = @mid
);