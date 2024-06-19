package com.punpuf.ssc0103fifa_database_client.utils;

import java.util.HashMap;
import java.util.Map;

/**
 * Helper class to create pairs between country names and their codes.
 * <p>
 * Creates pair between a country code, as defined in the country-flags repository,
 * and the name used in the .CSV file that specifies the FIFA players information.
 * </p>
 * <p>
 * Seeking to avoid the repetitive work of creating these pairings manually,
 * this processed was automated using a LLM. Then edited to fix issues.
 * </p>
 *
 */
public class CountryCodeFinder {
    private static final Map<String, String> countryCodeMap = new HashMap<>();

    static {
        countryCodeMap.put("ALBANIA", "AL");
        countryCodeMap.put("ALGERIA", "DZ");
        countryCodeMap.put("ARGENTINA", "AR");
        countryCodeMap.put("AUSTRALIA", "AU");
        countryCodeMap.put("AUSTRIA", "AT");
        countryCodeMap.put("BELGIUM", "BE");
        countryCodeMap.put("BOLIVIA", "BO");
        countryCodeMap.put("BOSNIA AND HERZEGOVINA", "BA");
        countryCodeMap.put("BRAZIL", "BR");
        countryCodeMap.put("BURKINA FASO", "BF");
        countryCodeMap.put("CAMEROON", "CM");
        countryCodeMap.put("CANADA", "CA");
        countryCodeMap.put("CAPE VERDE ISLANDS", "CV");
        countryCodeMap.put("CHILE", "CL");
        countryCodeMap.put("CHINA PR", "CN");
        countryCodeMap.put("COLOMBIA", "CO");
        countryCodeMap.put("CONGO DR", "CD");
        countryCodeMap.put("COSTA RICA", "CR");
        countryCodeMap.put("COTE D'IVOIRE", "CI");
        countryCodeMap.put("CROATIA", "HR");
        countryCodeMap.put("CYPRUS", "CY");
        countryCodeMap.put("CZECH REPUBLIC", "CZ");
        countryCodeMap.put("DENMARK", "DK");
        countryCodeMap.put("DOMINICAN REPUBLIC", "DO");
        countryCodeMap.put("ECUADOR", "EC");
        countryCodeMap.put("EGYPT", "EG");
        countryCodeMap.put("ENGLAND", "GB");
        countryCodeMap.put("EQUATORIAL GUINEA", "GQ");
        countryCodeMap.put("FINLAND", "FI");
        countryCodeMap.put("FRANCE", "FR");
        countryCodeMap.put("GABON", "GA");
        countryCodeMap.put("GAMBIA", "GM");
        countryCodeMap.put("GERMANY", "DE");
        countryCodeMap.put("GHANA", "GH");
        countryCodeMap.put("GREECE", "GR");
        countryCodeMap.put("GUINEA BISSAU", "GW");
        countryCodeMap.put("HAITI", "HT");
        countryCodeMap.put("HONDURAS", "HN");
        countryCodeMap.put("HUNGARY", "HU");
        countryCodeMap.put("ICELAND", "IS");
        countryCodeMap.put("IRAN", "IR");
        countryCodeMap.put("ISRAEL", "IL");
        countryCodeMap.put("ITALY", "IT");
        countryCodeMap.put("JAMAICA", "JM");
        countryCodeMap.put("JAPAN", "JP");
        countryCodeMap.put("KAZAKHSTAN", "KZ");
        countryCodeMap.put("KENYA", "KE");
        countryCodeMap.put("KOREA REPUBLIC", "KR");
        countryCodeMap.put("KOSOVO", "XK");
        countryCodeMap.put("LIBYA", "LY");
        countryCodeMap.put("LIECHTENSTEIN", "LI");
        countryCodeMap.put("LUXEMBOURG", "LU");
        countryCodeMap.put("MADAGASCAR", "MG");
        countryCodeMap.put("MALI", "ML");
        countryCodeMap.put("MEXICO", "MX");
        countryCodeMap.put("MONTENEGRO", "ME");
        countryCodeMap.put("MOROCCO", "MA");
        countryCodeMap.put("NETHERLANDS", "NL");
        countryCodeMap.put("NEW ZEALAND", "NZ");
        countryCodeMap.put("NIGERIA", "NG");
        countryCodeMap.put("NORTH MACEDONIA", "MK");
        countryCodeMap.put("NORTHERN IRELAND", "GB-NIR");
        countryCodeMap.put("NORWAY", "NO");
        countryCodeMap.put("PARAGUAY", "PY");
        countryCodeMap.put("PERU", "PE");
        countryCodeMap.put("POLAND", "PL");
        countryCodeMap.put("PORTUGAL", "PT");
        countryCodeMap.put("QATAR", "QA");
        countryCodeMap.put("REPUBLIC OF IRELAND", "IE");
        countryCodeMap.put("ROMANIA", "RO");
        countryCodeMap.put("RUSSIA", "RU");
        countryCodeMap.put("SAUDI ARABIA", "SA");
        countryCodeMap.put("SCOTLAND", "GB-SCT");
        countryCodeMap.put("SENEGAL", "SN");
        countryCodeMap.put("SERBIA", "RS");
        countryCodeMap.put("SLOVAKIA", "SK");
        countryCodeMap.put("SLOVENIA", "SI");
        countryCodeMap.put("SOUTH AFRICA", "ZA");
        countryCodeMap.put("SPAIN", "ES");
        countryCodeMap.put("SURINAME", "SR");
        countryCodeMap.put("SWEDEN", "SE");
        countryCodeMap.put("SWITZERLAND", "CH");
        countryCodeMap.put("SYRIA", "SY");
        countryCodeMap.put("THAILAND", "TH");
        countryCodeMap.put("TOGO", "TG");
        countryCodeMap.put("TRINIDAD AND TOBAGO", "TT");
        countryCodeMap.put("TUNISIA", "TN");
        countryCodeMap.put("TURKEY", "TR");
        countryCodeMap.put("UKRAINE", "UA");
        countryCodeMap.put("UNITED STATES", "US");
        countryCodeMap.put("URUGUAY", "UY");
        countryCodeMap.put("UZBEKISTAN", "UZ");
        countryCodeMap.put("VENEZUELA", "VE");
        countryCodeMap.put("WALES", "GB-WLS");
        countryCodeMap.put("ZIMBABWE", "ZW");
    }

    public static String getCountryCode(String country) {
        return countryCodeMap.get(country.toUpperCase());
    }

}
