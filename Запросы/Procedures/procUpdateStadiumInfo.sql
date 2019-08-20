--use fifaDataBase;

--GO
--IF OBJECT_ID('dbo.UpdateStadiumInfo', 'U') IS NOT NULL  
--	DROP PROC dbo.UpdateStadiumInfo; 

--GO
--CREATE PROC dbo.UpdateStadiumInfo(@pid int, @town nvarchar(30), @name nvarchar(30), @seatcount int, @sectorcount int, @scheme image)
--AS
--	UPDATE dbo.StadiumInfo 
--	SET sname = @name, sseatcount = @seatcount, tsectorcount = @sectorcount, tscheme = @scheme
--	WHERE pid = @pid AND stown LIKE @town

--GO

EXEC dbo.UpdateStadiumInfo 0, 'Москва', 'Открытие арена', 10, 20, 0x;
GO
SELECT * FROM dbo.StadiumInfo;