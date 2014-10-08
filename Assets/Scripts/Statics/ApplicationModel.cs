using UnityEngine;
using System.Collections;
using System.Xml.Serialization;
using System.IO;
using System;
using System.Collections.Generic;

namespace RR2.Statics {
	public class ApplicationModel : Singleton<ApplicationModel> {
		protected ApplicationModel () {}

		public Map MapData;

		public float TileSize = 4.0f;
		public float TileHeight = 4.0f;

		public List<bool> TakenData;

		private bool disposed = false;

		void Start() {
			TakenData = new List<bool> ();
			DontDestroyOnLoad (this);
		}

		public void LoadMap(string path) {
			XmlSerializer serialiser = new XmlSerializer (typeof(Map));

			StreamReader reader = new StreamReader (path);
			MapData = (Map)serialiser.Deserialize (reader);
			reader.Close ();

			Application.LoadLevel ("Game");
		}

		public void Check() {
			TakenData.Add (true);
			if (TakenData.Count >= 1) {
				//Dispose();
				Destroy(this);
			}
		}
	}


}