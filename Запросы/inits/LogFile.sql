USE fifaDataBase;

ALTER DATABASE fifaDataBase 
ADD LOG FILE 
(
    NAME = fifaDataBaseLog,
    FILENAME = 'C:\Program Files\Microsoft SQL Server\MSSQL13.SQLSERVER\MSSQL\DATA\fifaDataBaseLog.ldf',
    SIZE = 5MB,
    MAXSIZE = 100MB,
    FILEGROWTH = 5MB
);