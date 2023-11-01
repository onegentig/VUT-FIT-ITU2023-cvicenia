using System.Windows;
using WPFWeather.App.Services;
using WPFWeather.App.ViewModels;

namespace WPFWeather.App.Windows
{
    public partial class MainWindow : Window
    {
        public MainWindow()
        {
            InitializeComponent();

            /* ViewModel – Offline data */
            WeatherViewModel viewmodel = new(new WeatherDiskService());
            // viewModel.DownloadWeatherCommand.Execute("Brno");

            /* ViewModel – Online data */
            //WeatherViewModel viewModel = new(new WeatherOpenWeatherMapService());

            DataContext = viewModel;
        }
    }
}