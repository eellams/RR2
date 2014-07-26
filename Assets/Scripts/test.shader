Shader "Custom/test" {
	Properties {
		_Color ("Main Color", Color) = (1,1,1,1)
		_MainTex ("Base (RGB)", 2D) = "white" {}
	}
	SubShader {
		Tags { "RenderType"="Opaque" }
		LOD 200
		
		CGPROGRAM
		#pragma surface surf Lambert
		sampler2D _MainTex;
		fixed4 _Color;

		struct Input {
			float2 uv_MainTex;
			half4 color : COLOR0;
		};

		void surf (Input IN, inout SurfaceOutput o) {
			half4 c = tex2D (_MainTex, IN.uv_MainTex);
			o.Albedo = c.rgb * _Color.rgb * IN.color.rgb;
			o.Alpha = c.a;
		}
		ENDCG
		
		Pass {
			Material {
				Diffuse (1,1,1,1)
				}
			Lighting On
			Cull Front
		}
	} 
	FallBack "Diffuse"
}
