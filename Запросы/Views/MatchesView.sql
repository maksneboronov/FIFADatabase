use fifaDataBase;

GO

CREATE VIEW MatchesView
AS
SELECT mid, tid1 as hteam, tid2 as ateam, mdate as matchDate, mg1 as gH, mg2 as gA, stid FROM dbo.Matches;