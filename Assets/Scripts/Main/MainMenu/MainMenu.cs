using UnityEngine;
using System.Collections;
using System.IO;
using System.Collections.Generic;
using RR2.Statics;

namespace RR2.MainMenu {
	public class MainMenu : MonoBehaviour {
		public string StartText;

		private string[] FileEntries;

		// Use this for initialization
		void Start () {
			if (!Directory.Exists ("Data")) {
				Debug.Log ("No Data folder!");
				StartText = "No Data folder";
				FileEntries = new string[0];
			}
			else if (!File.Exists("Data/mapformat.xsd")) {
				Debug.Log("No mapformat.xml");
				StartText = "No mapformat.xml";
				FileEntries = new string[0];
			}
			else {
				FileEntries = Directory.GetFiles("Data", "*.xml");
				StartText = "Main menu";
			}
		}

		// Update is called once per frame
		void Update () {
		
		}

		void OnGUI() {
			GUI.Box (new Rect (10, 10, 200, 30*FileEntries.Length + 50), StartText);

			for (int i=0; i<FileEntries.Length; i++) {
				if (GUI.Button (new Rect(20, 30*i+40, 180, 20), FileEntries[i])) {
					Debug.Log(FileEntries[i]);

					//TODO loading screen

					ApplicationModel.Instance.LoadMap(FileEntries[i]);
				}
			}
		}
	}
}
