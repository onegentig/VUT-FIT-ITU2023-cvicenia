# Pokročilé nástroje a knihovny pro Windows (WPF, .NET) #

## Zadání ##

Vytvořte pomocí WPF (XAML + C#) aplikaci zobrazující aktuální počásí v daném městě.

Aplikace musí zobrazovat:

- Jméno města
- Teplotu
- Vlhkost
- Rychlost větru
- Směr větru (vizualizovat, text nestačí)
- Popis počasí (cloudy, clear, sunny...)
- Ikona počasí

### Hodnocení zadání ###

Základ (3 body):

- Vzhled v XAML za použití relativního pozicování, tzn. aby to vypadalo dobře při různých velikostech okna **(1b)**
- Správné vytvoření data bindingu (vytvoření instance ViewModelu, jeho nastavení jako DataContext okna a nastavení bindingu u prvků GUI) **(1b)**
- Grafická vizualizace směru větru (např. šipka, trojúhelník, obrázek) **(1b)**

Pokračování (2 body). Rozšiřte předchozí zadání o následující vlastnosti:

- Město (Brno, Praha nebo London) musí jít změnit přímo v programu (ne jen v kódu) **(1b)**
- Aplikace musí informovat uživatele o tom, že se data právě stahují (použijte již naimplementovaný BoolToVisibilityConverter) **(1b)**

## Pokyny k vypracování ##

1. Stáhněte si a rozbalte [kostru programu](https://moodle.vut.cz/pluginfile.php/597574/mod_page/content/27/kostra.zip)
2. Stáhněte si [přednášku o WPF](https://moodle.vut.cz/mod/resource/view.php?id=320730)
3. Otevřete soubor ITU-Lab.sln ve Visual Studiu
   - Pokud při otevírání dostanete "Unspecified error", zkuste cvičení otevřít přes menu "File -> Open -> Project/Solution..."
4. Hotové cvičení odevzdejte do ISu - před odevzdáním smažte složky `bin` a `obj`

### Ukázka ###

<div markdown="1" width="223" height="355">

![Ukázka](https://github.com/onegentig/VUT-FIT-ITU2023-cvicenia/assets/84882649/3cdeb63f-ae4e-47a1-93c5-26a1b6db847f)

## Co používat? ##

- Berte jenom jako návod, co googlit! Můžete samozřejmě použít, co chcete.
- Grid, StackPanel, ListBox, Button, TextBox, TextBlock, DataTemplate

### Použití Grid ###

```xml
<Grid>
  <Grid.ColumnDefinitions>
      <ColumnDefinition Width="100" />
      <ColumnDefinition Width="*" />
      <ColumnDefinition Width="80" />
  </Grid.ColumnDefinitions>
  <Grid.RowDefinitions>
      <RowDefinition Height="*" />
      <RowDefinition Height="50" />
  </Grid.RowDefinitions> 
  
  <!-- Your stuff here -->
    
</Grid>
```

[Grid Panel tutoriál](http://wpftutorial.net/GridLayout.html)

### Vytvoření ViewModelu a stažení počasí ###

V kostře projektu je třída pro `ViewModel` již vytvořená. Jmenuje se `WeatherViewModel` a je ve složce/namespacu `ViewModel`. V hlavním okně aplikace ji stačí v konstruktoru vytvořit a uložit například jako člena třídy.

```csharp
WeatherViewModel viewmodel = new WeatherViewModel(new WeatherDiskService());

viewmodel.DownloadWeatherCommand.Execute("Brno"); 
```

#### Užiteční členové třídy WeatherViewModel ####

- **DownloadingWeather** - True when weather is currently being downloaded from online service.
- **Weather** - Weather info (like temperature, humidity etc.)

#### Binding ####

Před samotným bindingem je nutné správně nastavit `DataContext` hlavního okna.

```csharp
public partial class MainWindow : Window
{
    public MainWindow()
    {
        InitializeComponent();

        // Set DataContext of this window to main ViewModel of application
        DataContext = new WeatherViewModel(new WeatherDiskService());
    }

    // ...
} 
```

Bindování vlastností třídy `WeatherViewModel` na vlastnost `Text` prvku uživatelského rozhraní.

Pozor – formátování textu funguje pro `TextBlock`, ale ne pro `TextBox` (pokud se vám nezobrazuje, zkontrolujte si jestli omylem nepoužíváte `TextBox`).

```xml
<TextBlock Text="{Binding Weather.CityName}" />
<TextBlock Text="{Binding Weather.Temperature, StringFormat={}{0:0}°}" />
```

### Kreslení ###

Pro vizualizaci směru větru je možné využít například třídu `Polygon`.

```xml
<Polygon Width="31" Height="31" Fill="White">
  <Polygon.LayoutTransform>
      <RotateTransform Angle="45" />
  </Polygon.LayoutTransform>
  <Polygon.Points>
      <Point X="15" Y="0" />
      <Point X="10" Y="30" />
      <Point X="20" Y="30" />
  </Polygon.Points>
</Polygon>
```

### ComboBox a InvokeCommandAction ###

Pro výběr prvku z `ComboBox` použijeme `InvokeCommandAction` z knihovny `Microsoft.Toolkit.Mvvm` vyvolaný za pomoci `<b:Interaction.Triggers />`.

Do `ComboBox`u přidejte jednotlivé položky s názvy měst (pro toto cvičení je stačí přidat ručně přímo do kódu) a doplňte bindy pro zavolání správného commandu.

```xml
<ComboBox x:Name="CitySelectComboBox" SelectedValuePath="Content" IsEditable="True" Width="100">
  <b:Interaction.Triggers>
      <b:EventTrigger EventName="Loaded">
          <b:InvokeCommandAction Command="{Binding FIXME}"
                                 CommandParameter="{Binding ElementName=CitySelectComboBox, Path=SelectedValue}" />
      </b:EventTrigger>
      <b:EventTrigger EventName="SelectionChanged">
          <b:InvokeCommandAction Command="{Binding FIXME}"
                                 CommandParameter="{Binding ElementName=CitySelectComboBox, Path=SelectedValue}" />
      </b:EventTrigger>
  </b:Interaction.Triggers>
</ComboBox>
```

### Binding – Converter ###

V projektu je přiložena třída `BoolToVisibilityConverter`, která konvertuje True a False na Visibility (Hidden, Collapsed, Visible).

```xml
<Grid Visibility="{Binding DownloadingWeather, Converter={StaticResource boolToVisibilityConverter}, ConverterParameter=False}">
...
</Grid>
```

## Odevzdání ##

Do ISu odevzdejte vše **kromě složek bin a obj**!

## Odkazy ##

- [XAML Overview](https://msdn.microsoft.com/en-us/library/ms752059%28v=vs.100%29.aspx)
- [Data Binding](http://msdn.microsoft.com/en-us/library/ms752347(v=vs.100).aspx)
- [Grid - Part 1 (klikněte na next pro další části)](http://www.wpf-tutorial.com/panels/grid/)
- [Shapes and Basic Drawing in WPF Overview](https://msdn.microsoft.com/en-us/library/ms747393%28v=vs.100%29.aspx)

--------------------

Naposledy změněno: středa, 9. listopadu 2022, 13.18

