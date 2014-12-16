using UnityEngine;
using System.Collections;
using EMK.Cartography;

public class RrAStar : MonoBehaviour {

	/*private Graph WaterGraph;
	private Graph LandGraph;
	private Graph AirGraph;*/

	private Graph TheGraph;

	// Use this for initialization
	void Awake () {
		/*WaterGraph = new Graph ();
		LandGraph = new Graph ();
		AirGraph = new Graph ();*/
		TheGraph = new Graph ();
		Initialise ();
	}
	
	// Update is called once per frame
	void Update () {
	
	}

	public void Initialise() {
		for (int i=0; i<MapController.Instance.Width*MapController.Instance.Height; i++) {
			int x, y;

			x = i%MapController.Instance.Width;
			y = i/MapController.Instance.Width;

			uint moveType = 0;
			if (MapController.Instance.GetTile(i).NewMoveType.Contains(LogicController.MoveType.Land.ToString()))
			    moveType |= 1;
			if (MapController.Instance.GetTile(i).NewMoveType.Contains(LogicController.MoveType.Air.ToString()))
			    moveType |= 2;
			if (MapController.Instance.GetTile(i).NewMoveType.Contains(LogicController.MoveType.Water.ToString()))
			    moveType |= 4;

			/*WaterGraph.AddNode(new Node((double)(x+2), 0, (double)(y+2)));
			LandGraph.AddNode(new Node((double)(x+2), 0, (double)(y+2)));
			AirGraph.AddNode(new Node((double)(x+2), 0, (double)(y+2)));*/
			TheGraph.AddNode((float)(x+2), 0.0f, (float)(y+2), moveType);
		}

		for (int i=0; i<MapController.Instance.Width*MapController.Instance.Height; i++) {
			if (i < MapController.Instance.Width - 1) {
				//TheGraph.Add2Arcs(TheGraph.Nodes[i], TheGraph.Nodes[i+1], 1);
			}
		}

	}
}
