use fifaDataBase;


--CREATE VIEW MatchViewWithGroup
--AS
--SELECT  m.mid, m.mdate, m.Stid, t1.tgroup, t1.tname as hteam, t2.tname as ateam, m.mg1, m.mg2 
--FROM dbo.Matches as m INNER JOIN dbo.Teams as t1 ON m.tid1 = t1.tid INNER JOIN dbo.Teams as t2 ON m.tid2 = t2.tid;