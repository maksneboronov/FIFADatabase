using System;
using System.Data.SqlClient;
using System.IO;
using System.Net;
using System.Net.Sockets;
using System.Text;
using System.Threading;
using System.Threading.Tasks;

namespace FTPServer
{
	public class Translater
	{
		public Translater()
		{
			_connectionString = @"Data Source=MAKS\SQLSERVER; Initial Catalog=master; Integrated Security=SSPI;";
			_connection = new SqlConnection(_connectionString);
			_connection.Open();
		}

		~Translater()
		{
			if (_connection.State == System.Data.ConnectionState.Open)
			{
				_connection.Close();
			}
		}

		private string Backup()
		{
			var date = DateTime.Now.ToString();
			var filename = $"F:\\backups\\backupFDB_{date.ToString().Replace(' ', '_').Replace('.', '_').Replace(':', '_') + "_" + date.GetHashCode().ToString()}.bat";
			var backup = $"BACKUP DATABASE fifaDataBase TO DISK = '{filename}' ";
			Console.WriteLine(filename);
			using (var comm = new SqlCommand(backup, _connection))
			{
				comm.ExecuteNonQuery();
			}
			return filename;
		}

		private void Restore(string filename)
		{
			var queries = new string[]
			{
				"ALTER DATABASE fifaDataBase SET SINGLE_USER WITH ROLLBACK IMMEDIATE;",
				$"RESTORE DATABASE fifaDataBase FROM DISK = '{filename}' ",
				"ALTER DATABASE fifaDataBase SET MULTI_USER"
			};
			foreach (var item in queries)
			{
				Console.WriteLine(item);
				using (var comm = new SqlCommand(item, _connection))
				{
					comm.ExecuteNonQuery();
				}
			}
		}

		public async void StartTranslate(TcpClient inClientSocket, ulong clientNubmer)
		{
			this._clientSocket = inClientSocket;
			this._clientSocket.ReceiveTimeout = 5 * 1000;
			this._clientNumber = clientNubmer;
			await Task.Run(() => Translating());
		}

		private void Translating()
		{
			var bytes = new byte[size];

			try
			{
				var bytesRec = _clientSocket.Client.Receive(bytes);
				var data = Encoding.UTF8.GetString(bytes, 0, bytesRec);
				Console.Write($"CN( {_clientNumber} ): Command: {data} \n\n");
				var command = data.ToLower();
				_clientSocket.Client.Send(Encoding.UTF8.GetBytes("OK"));
				if (command == "backup")
				{
					Console.Write($"CN( {_clientNumber} ): Begin backup \n\n");
					var filename = Backup();
					_clientSocket.Client.Receive(bytes);
					using (var file = new FileStream(filename, FileMode.Open, FileAccess.Read, FileShare.Read))
					{
						_clientSocket.Client.Send(BitConverter.GetBytes(file.Length));
						_clientSocket.Client.Receive(bytes);
						using (var f = new BinaryReader(file))
						{
							while (file.Position < file.Length)
							{
								var res = f.ReadBytes(size);
								_clientSocket.Client.Send(res, res.Length, SocketFlags.None);
								_clientSocket.Client.Receive(bytes);
							}
						}
					}
					File.Delete(filename);
					Console.Write($"CN( {_clientNumber} ): End backup: {data} \n\n");
				}
				else if (command == "restore")
				{
					Console.Write($"CN( {_clientNumber} ): Begin restore \n\n");					
					_clientSocket.Client.Receive(bytes);
					var size = BitConverter.ToInt64(bytes, 0);
					Console.WriteLine(size);
					var date = DateTime.Now.ToString();
					var filename = $"F:\\restores\\restoreFDB_{date.ToString().Replace(' ', '_').Replace('.', '_').Replace(':', '_') + "_" + date.GetHashCode().ToString()}.bat";
					var i = 0;
					using (var file = new FileStream(filename, FileMode.Create, FileAccess.Write))
					{
						using (var f = new BinaryWriter(file))
						{
							while (i < size)
							{
								var br = _clientSocket.Client.Receive(bytes);
								_clientSocket.Client.Send(Encoding.UTF8.GetBytes("OK"));
								f.Write(bytes, 0, br);
								i += br;
							}
						}
					}
					if (i == size)
					{
						Restore(filename);
					}
					File.Delete(filename);
					Console.Write($"CN( {_clientNumber} ): End restore \n\n");
				}
			}
			catch (Exception ex)
			{
				Console.WriteLine(ex.Message);
			}
			finally
			{
				_clientSocket.Client.Shutdown(SocketShutdown.Both);
			}
			Console.Write($" >> Client No: {_clientNumber} shutdown!\n\n");
		}

		private TcpClient _clientSocket;
		private ulong _clientNumber;

		private const int size = 1024;

		private readonly string _connectionString;
		private SqlConnection _connection;
	}


	class Program
	{
		static void Main(string[] args)
		{
			var ipHost = Dns.GetHostEntryAsync("localhost").Result;
			var ipAddr = ipHost.AddressList[0];
			var serverSocket = new TcpListener(new IPEndPoint(ipAddr, 11000));
			var clientSocket = default(TcpClient);

			serverSocket.Start();
			Console.WriteLine(" >> " + "Server Started");

			ulong counter = 0;
			while (true)
			{
				try
				{
					if (counter + 1 == ulong.MaxValue)
					{
						counter = 0;
					}
					counter += 1;
					clientSocket = serverSocket.AcceptTcpClientAsync().Result;
					Console.WriteLine(" >> " + "Client No:" + Convert.ToString(counter) + " started!");
					var client = new Translater();
					client.StartTranslate(clientSocket, counter);
				}
				catch
				{
					break;
				}
			}

			serverSocket.Stop();
			Console.WriteLine(" >> " + "exit");
			Console.ReadLine();
		}

	}
}
