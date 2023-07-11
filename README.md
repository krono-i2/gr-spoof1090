# Spoof1090

Spoof 1090 is a ADS-B transmitter specifically designed for GNU Radio implementations.

## Description

Spoof 1090 is composed by two sub-modules:

* the ADS-B encoder: a C++ tool aimed at providing the ADS-B payload based on the state of the aircraft (i.e., ICAO name, position);
* the ADS-B modulator: a GNU Radio block aimed at providing the modulated burst to be transmitted.

## Acknowledgments

This project has been developed within the collaboration between the Safty and Security department of the [Italian Aerospace Research Centre (CIRA)](https://www.cira.it/en) and the [SPRINT](https://sprint.dieti.unina.it/index.php/en) research group.

If you find this project useful for your research, please considering cite this tool as:

G. Gelli, I. Iudice and D. Pascarella, "A cloud-assisted ADS-B network for UAVs based on SDR," *2022 IEEE 9th International Workshop on Metrology for AeroSpace (MetroAeroSpace)*, Pisa, Italy, 2022, pp. 7-12, doi: [10.1109/MetroAeroSpace54187.2022.9856398](https://doi.org/10.1109/MetroAeroSpace54187.2022.9856398).

You can find the bibtex code below:

```
@INPROCEEDINGS{9856398,
  author={Gelli, Giacinto and Iudice, Ivan and Pascarella, Domenico},
  booktitle={2022 IEEE 9th International Workshop on Metrology for AeroSpace (MetroAeroSpace)}, 
  title={A cloud-assisted ADS-B network for UAVs based on SDR}, 
  year={2022},
  pages={7-12},
  doi={10.1109/MetroAeroSpace54187.2022.9856398}
}
```
