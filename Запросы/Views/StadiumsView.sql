use fifaDataBase;

GO

CREATE VIEW StadiumsView
AS
SELECT stid as id, pid, sname as stName, stown as town, sseatcount as seatCount, tsectorcount as sectorCount, tscheme as scheme FROM dbo.Stadium;

