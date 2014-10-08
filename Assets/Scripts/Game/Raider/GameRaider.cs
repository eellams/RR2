using UnityEngine;
using System.Collections;
using RR2.Statics;
using System.Reflection;

namespace RR2.Raider_ {
	public class GameRaider : MonoBehaviour {
		public _GameRaider GR;

		public int RaiderId { get { return GR.RaiderId; } }
		public string Name { get { return GR.Name; } }

		void Awake() {
			GR = new _GameRaider (ApplicationModel.Instance.MapData.RockRaider);
			GameObject selectGO = transform.FindChild ("Select").gameObject;
			selectGO.SetActive (false);
		}

		public void Select() {
			Debug.Log(string.Format("Selecting Raider {0}", RaiderId));
			transform.FindChild ("Select").gameObject.SetActive (true);
		}

		public void Deselect() {
			Debug.Log (string.Format ("Deselecting Raider {0}", RaiderId));
			transform.FindChild ("Select").gameObject.SetActive (false);
		}

		void Update() {

		}
	}


	public class _GameRaider : RockRaider {
		public GameObject Prefab;
		
		public _GameRaider(RockRaider c) : base()
		{
			// copy base class properties.
			foreach (PropertyInfo prop in c.GetType().GetProperties())
			{
				PropertyInfo prop2 = c.GetType().GetProperty(prop.Name);
				prop2.SetValue(this, prop.GetValue(c, null), null);
			}
			
			Prefab = (GameObject)Resources.Load (this.ModelPath);
		}
		
		public void InheritFromRaider(Raider t) {
			foreach (PropertyInfo property in t.GetType().GetProperties( BindingFlags.Public | BindingFlags.Instance))
			{
				PropertyInfo targetProperty = this.GetType().GetProperty( property.Name, BindingFlags.Public | BindingFlags.Instance );
				if (targetProperty != null
				    && targetProperty.CanWrite)
				{
					targetProperty.SetValue( this, property.GetValue(t, null), null );
				}
			}
		}
	}
}