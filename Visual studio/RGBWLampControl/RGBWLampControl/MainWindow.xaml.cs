using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;

namespace RGBWLampControl 
{
    public class ViewModel : INotifyPropertyChanged
    {
        private String status;

        public String Status
        {
            get { return status; }
            set
            {
                status = value;
                NotifyPropertyChanged("Status");
            }
        }

        public event PropertyChangedEventHandler PropertyChanged;

        public void NotifyPropertyChanged(string propertyName)
        {
            if (PropertyChanged != null)
            {
                PropertyChanged(this, new PropertyChangedEventArgs(propertyName));
            }
        }

        public ViewModel(string InitText)
        {
            this.Status = InitText;
        }
    }

    public partial class MainWindow : Window
    {
        RGBWLamp MyLamp = new RGBWLamp("RGBW","WBGR");

        ViewModel vm = new ViewModel("Awaiting for connection.");        


        public MainWindow()
        {
            InitializeComponent();
            this.DataContext = vm;
        }

        private void button_Click(object sender, RoutedEventArgs e)
        {
            try
            {
                if (MyLamp.Start())
                {
                    vm.Status = "Successfully connected to Lamp !";
                }
            }
            catch (Exception Error)
            {
                vm.Status = Error.Message;
            }
        }

        private void SliderValueUpdate(object sender, RoutedPropertyChangedEventArgs<double> e)
        {
            Slider TempSlider = sender as Slider;
            MyLamp.writeHSI((int)HueSlider.Value, (int)SaturationSlider.Value, (int)BrightnessSlider.Value);
        }

        private void button1_Click(object sender, RoutedEventArgs e)
        {
            MyLamp.fade();
        }
    }
}
