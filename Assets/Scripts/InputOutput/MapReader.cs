using System.Collections;
using System.Collections.Generic;
using System.Xml.Serialization;
using System.IO;
using UnityEngine;
using System;

public class MapReader : MonoBehaviour {
	public Map LoadMap(string mapDir) {
		XmlSerializer serializer = new XmlSerializer (typeof(Map));
		serializer.UnknownNode += new XmlNodeEventHandler (serializer_UnknownNode);
		serializer.UnknownAttribute += new XmlAttributeEventHandler (serializer_UnknownAttribute);
		
		FileStream fs = new FileStream (mapDir, FileMode.Open);

		Map toReturn = (Map)serializer.Deserialize (fs);

		fs.Close ();

		return toReturn;
	}

	void serializer_UnknownNode(object sender, XmlNodeEventArgs e)
	{
		Debug.Log("Unknown Node:" + e.Name + "\t" +e .Text);
	}
	
	void serializer_UnknownAttribute(object sender, XmlAttributeEventArgs attr)
	{
		Debug.Log("Unknown Attribute:" + attr.Attr.Name + "='" + attr.Attr.Value + "'");
	}
}

[XmlRoot("Map")]
public class Map {
	public string Name;
	public int Width;
	public int Height;

	[XmlArrayAttribute("Blocks")]
	public Block[] Blocks;

	[XmlArrayAttribute("TileTypes")]
	public TileType[] TileTypes;

	[XmlIgnore]
	public TileType[] Tiles;

	[XmlIgnore]
	public List<float> HeightBase;

	public void Setup(Material mat) {
		Array.Sort (Blocks, (x, y) => x.ID.CompareTo (y.ID));

		Dictionary<int, TileType> tileTypeDict = new Dictionary<int, TileType>();

		foreach (TileType tType in TileTypes) {
			FileStream fs = new FileStream (tType.Tex, FileMode.Open);
			byte[] imageContents = new byte[fs.Length];

			fs.Read (imageContents, 0, imageContents.Length);
			fs.Close ();

			Texture2D tempTex = new Texture2D(1,1);
			tempTex.LoadImage (imageContents);

			tType.Mat = new Material(mat);
			tType.Mat.SetTexture ("_MainTex", tempTex);

			tileTypeDict.Add (tType.TileId, tType);
		}	

		Tiles = new TileType[Blocks.Length];
			
		for (int i=0; i<Blocks.Length; i++) {
			Tiles[i] = (TileType)tileTypeDict[Blocks[i].TileId].Clone ();

			if (Blocks[i].Height > 0)
				Tiles[i].Height = Blocks[i].Height;
			if (Blocks[i].EC >= 0)
				Tiles[i].EC = Blocks[i].EC;
			if (Blocks[i].ORE >= 0)
				Tiles[i].ORE = Blocks[i].EC;

   		}
		CreateHeightBase ();
		RecalculateSurround ();
	}

	private void CreateHeightBase() {
		List<float> heightbase = new List<float> ();
		heightbase.Add ((float)Tiles[0].Height);
		
		for (int i = 0; i<Width; i++) {
			heightbase.Add (Tiles[i].Height);
		}
		
		for (int i=0; i<Height-1; i++) {
			heightbase.Add (Tiles [Width * i].Height);
			for (int j=0; j<Width-1; j++) {
				float height = (float)(Tiles [Width * i + j].Height +
				                       Tiles [Width * i + j + 1].Height +
				                       Tiles [Width * i + j + Width].Height +
				                       Tiles [Width * i + j + 1 + Width].Height) / 4;
				heightbase.Add (height);
			}
			
			heightbase.Add ((Tiles [Width * i + Width - 1].Height +
			                 Tiles [Width * i + 2 * Width - 1].Height) / 2.0f);
		}
		
		for (int i = Width; i>0; i--) {
			heightbase.Add (Tiles[Tiles.Length - i].Height);
		}
		
		HeightBase = heightbase;
	}

	private void RecalculateSurround() {
		//   1|..|64 
		//  --------
		//   2| X|32
		//  --------
		//   4| 8|16
		
		for (int i = 0; i<Tiles.Length; i++) {
			byte surround = 0;
			
			if (i % Width == Width - 1) // On the right hand side of the map
				surround |= 0x70;
			else if (i<Tiles.Length - 1) { // A solid can be to the right (not at the edge)
				if (Tiles[i+1].Solid) // There is a solid on the right
					surround |= 0x20;
			}
			
			if (i<Tiles.Length - Width - 1) { // If top right exists
				if (Tiles[i+Width+1].Solid) // If top right solid
					surround |= 0x40;
			}
			
			if (i<Tiles.Length - Width) { // If top left exists
				if (Tiles[i+Width-1].Solid) // If top left solid
					surround |= 0x01;
			}
			
			if (i % Width == 0) // On the left hand side of the map
				surround |= 0x07;
			else if (i>0) { // A solid can be on the left
				if (Tiles[i-1].Solid) // A solid is on the left
					surround |= 0x02;
			}
			
			
			if (i>=Width) { // If solid can be on the bottom right
				if (Tiles[i-Width+1].Solid) // If bottom right solid
					surround |= 0x10;
			}
			
			if (i> Width + 1) { // If bottom left exists
				if (Tiles[i-Width-1].Solid) { // If bottom left solid
					surround |= 0x04;
				}
			}
			
			
			if (i / Width == 0) // On the bottom of the map
				surround |= 0x1C;
			else if (i >= Width) { // Not on the bottom row
				if (Tiles[i-Width].Solid) // If there is a solid below 
					surround |= 0x08;
			}
			
			if (i / Width == Height - 1) // On the top of the map
				surround |= 0xC1;
			else if (i < Tiles.Length - Width) { // Not on the top row
				if (Tiles[i+Width].Solid) // If solid tile in row above
					surround |= 0x80;
			}
			
			Tiles[i].Surround = (uint)surround;
		}
	}
}

public class Block : TileType {
	[XmlAttribute("TileId")]
	public int TileId;

	[XmlAttribute("Height")]
	public float Height;

	[XmlAttribute("EC")]
	public int EC;

	[XmlAttribute("ORE")]
	public int ORE;

	[XmlAttribute("ID")]
	public int ID;

	[XmlAttribute("Tex")]
	string Tex;

	// TODO Things like corrosion and avalanche and monster
}

public class TileType : ICloneable {
	public string Name;
	public int TileId;
	public float Height;
	public int EC;
	public int ORE;
	public bool Solid;

	public string Tex;

	[XmlIgnore]
	public UInt32 Surround;
	[XmlIgnore]
	public int ID;
	[XmlIgnore]
	public Material Mat;
	[XmlIgnore]
	public Mesh mesh;

	public object  Clone () {
		return this.MemberwiseClone ();
	}
}