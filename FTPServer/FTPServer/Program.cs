using System;
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
		public async void StartTranslate(TcpClient inClientSocket, int clientNubmer)
		{
			this._clientSocket = inClientSocket;
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
				Console.Write("Command: " + data + "\n\n");

				var command = data.ToLower();
				if (command == "backup")
				{
					using (var file = new FileStream("4курс.zip", FileMode.Open, FileAccess.Read, FileShare.Read))
					{
						_clientSocket.Client.Send(BitConverter.GetBytes(file.Length));
						using (var f = new BinaryReader(file))
						{
							while (file.Position < file.Length)
							{
								var res = f.ReadBytes(size);
								_clientSocket.Client.Send(res, res.Length, SocketFlags.None);
							}
						}
					}
				}
				else if (command == "restore")
				{
					_clientSocket.Client.Receive(bytes);
					var size = BitConverter.ToInt64(bytes, 0);

					using (var file = new FileStream("12.zip", FileMode.Create, FileAccess.Write))
					{
						using (var f = new BinaryWriter(file))
						{
							var i = 0;
							while (i < size)
							{
								var br = _clientSocket.Client.Receive(bytes);
								f.Write(bytes, 0, br);
								i += br;
							}
						}
					}
					Console.WriteLine("End");
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
		}

		private TcpClient _clientSocket;
		private int _clientNumber;

		private const int size = 1024;

		private readonly string _connectionString;
		private SqlConnection _connection;
	}


	class Program
	{
		public static async Task<IPHostEntry> GetHost(string str)
		{
			return await Dns.GetHostEntryAsync(str);
		}



		static void Main(string[] args)
		{
			IPHostEntry ipHost = GetHost("localhost").Result;
			IPAddress ipAddr = ipHost.AddressList[0];
			TcpListener serverSocket = new TcpListener(new IPEndPoint(ipAddr, 11000));
			TcpClient clientSocket = default(TcpClient);
			int counter = 0;

			serverSocket.Start();
			Console.WriteLine(" >> " + "Server Started");

			counter = 0;
			while (true)
			{
				try
				{
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
