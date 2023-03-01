using System;

namespace ScriptingSandbox
{

    public class CSharpTesting
    {
        public float MyPublicFloatVar = 1.0f;

        private string m_Name = "Hello";
        public string Name
        {
            get => m_Name;
            set
            {
                m_Name = value;
                MyPublicFloatVar += 5.0f;
            }
        }

        public void PrintFloatVar()
        {
            Console.WriteLine("MyPublicFloatVar = {0:F}", MyPublicFloatVar);
        }

        private void IncrementFloatVar(float value)
        {
            Console.WriteLine("Increment = {0:F}", value);
            MyPublicFloatVar += value;
        }

    }
}
